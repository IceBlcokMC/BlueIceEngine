#include "VMManager.h"
#include "Entry.h"
#include "vm/VM.h"

#include "vm/VMData.h"


#include <atomic>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


namespace bie {

class NodeJsInstance {
    static std::atomic_bool                     isInitialized_;
    std::vector<std::string>                    argv_;
    std::vector<std::string>                    execArgs_;
    std::unique_ptr<node::MultiIsolatePlatform> platform_;

public:
    NodeJsInstance()  = default;
    ~NodeJsInstance() = default;

    NodeJsInstance(NodeJsInstance const&)            = delete;
    NodeJsInstance& operator=(NodeJsInstance const&) = delete;
    NodeJsInstance(NodeJsInstance&&)                 = delete;
    NodeJsInstance& operator=(NodeJsInstance&&)      = delete;

    static NodeJsInstance& getInstance() {
        static NodeJsInstance* instance = new NodeJsInstance();
        return *instance;
    }

    bool isInitialized() const { return isInitialized_.load(); }

    node::MultiIsolatePlatform* platform() const { return platform_.get(); }

    std::vector<std::string> const& argv() const { return argv_; }

    std::vector<std::string> const& execArgs() const { return execArgs_; }

    bool initializeOnce() {
        bool expected = false;
        if (!isInitialized_.compare_exchange_strong(expected, true)) {
            return false;
        }

        auto  workingDir     = std::filesystem::current_path() / "bedrock_server.exe";
        char* charWorkingDir = (char*)workingDir.c_str();

        argv_.push_back(workingDir.string()); // cache argv

        uv_setup_args(1, &charWorkingDir); // init libuv

        auto result = node::InitializeOncePerProcess(
            argv_,
            {
                node::ProcessInitializationFlags::kNoInitializeV8,
                node::ProcessInitializationFlags::kNoInitializeNodeV8Platform,
            }
        );
        execArgs_ = result->exec_args();

        if (result->early_return() != 0) {
            Entry::getInstance()->getLogger().critical("Failed to initialize Node.js");
            for (auto& err : result->errors()) {
                Entry::getInstance()->getLogger().critical(err);
            }
            return false;
        }

        platform_ = node::MultiIsolatePlatform::Create((int)std::thread::hardware_concurrency());

        v8::V8::InitializePlatform(platform_.get());
        v8::V8::Initialize();
        return true;
    }

    // 由于 endstone 的重载机制是直接 delete plugin* 并 CLOSE_LIBRARY
    // 为了 Node.js 安全，Node.Js 和 v8 平台采用全局单例保活
    // 但这样导致 NodeJSInstance 析构必须游离于程序之外，且因 Windows Loader Lock 会造成死锁
    // 所以这里是已知的进程级内存泄漏，让操作系统清理吧（反正VM都停了，不影响脚本）
    // bool _shutdown() {
    //     if (!isInitialized_.exchange(false)) {
    //         return true;
    //     }
    //     v8::V8::Dispose();
    //     v8::V8::DisposePlatform();
    //     node::TearDownOncePerProcess();
    //     platform_.reset();
    //     return true;
    // }
};
decltype(NodeJsInstance::isInitialized_) NodeJsInstance::isInitialized_ = false;


struct VMManager::Impl {
    std::thread                                   uvThread_;
    std::mutex                                    uvExitMutex_;
    std::condition_variable                       uvExitCv_;
    std::atomic<bool>                             uvRunning_      = false;
    bool                                          uvThreadExited_ = false;
    VMID                                          nextID_         = 0;
    std::unordered_map<VMID, std::unique_ptr<VM>> vms_;
};

VMManager::VMManager() : impl(std::make_unique<Impl>()) {}

VMManager::~VMManager() = default;

bool VMManager::initialize() { return NodeJsInstance::getInstance().initializeOnce(); }

bool VMManager::shutdownVMs() {
    for (auto& [id, vm] : impl->vms_) {
        _performDestroy(vm.get());
    }
    impl->vms_.clear();
    return true;
}

void VMManager::initUvThread() {
    bool sou = false;
    impl->uvRunning_.compare_exchange_strong(sou, true);
    impl->uvThreadExited_ = false;

    impl->uvThread_ = std::thread([this]() {
        while (impl->uvRunning_) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            try {
                for (auto& [id, vm] : impl->vms_) {
                    if (vm->engine_->isDestroying()) {
                        continue;
                    }
                    v8kit::EngineScope lock{vm->engine_};
                    try {
                        uv_run(vm->nodeEnv_->event_loop(), UV_RUN_NOWAIT);
                    } catch (v8kit::Exception const& exc) {
                        Entry::getInstance()->getLogger().error(
                            "Error while running uv loop: {}\n{}",
                            exc.what(),
                            exc.stacktrace()
                        );
                    }
                }
            } catch (...) {
                Entry::getInstance()->getLogger().error("Error while running uv loop");
            }
        }
        {
            std::lock_guard lk(impl->uvExitMutex_);
            impl->uvThreadExited_ = true;
        }
        impl->uvExitCv_.notify_one();
    });
}
void VMManager::shutdownUvThread() {
    impl->uvRunning_ = false;

    if (impl->uvThread_.joinable()) {
        std::unique_lock lk(impl->uvExitMutex_);
        impl->uvExitCv_.wait(lk, [this] { return impl->uvThreadExited_; });
        lk.unlock();
        impl->uvThread_.join();
    }
}

bool VMManager::hasVM(VMID id) const { return impl->vms_.contains(id); }

VM* VMManager::createVM() {
    auto& node = NodeJsInstance::getInstance();
    if (!node.isInitialized()) {
        return nullptr;
    }

    auto const id = impl->nextID_++;

    std::vector<std::string> errors;
    auto                     env = node::CommonEnvironmentSetup::Create(
        node.platform(),
        &errors,
        node.argv(),
        node.execArgs(),
        node::EnvironmentFlags::Flags(
            // node::EnvironmentFlags::kNoCreateInspector // 禁用 inspector
            node::EnvironmentFlags::kOwnsProcessState
        )
    );
    if (!env) {
        auto& logger = Entry::getInstance()->getLogger();
        logger.error("Failed to create environment for VM {}", id);
        for (auto& err : errors) {
            logger.error(err);
        }
        return nullptr;
    }

    VM::V8Engine engine = nullptr;
    {
        auto isolate = env->isolate();

        v8::Locker         locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope    handle_scope(isolate);
        v8::Context::Scope context_scope(env->context());
        engine = new v8kit::Engine(isolate, env->context());

        node::AddEnvironmentCleanupHook(
            isolate,
            [](void* arg) {
                Entry::getInstance()->getLogger().debug("EnvironmentCleanupHook called, destroying engine [{}]", arg);
                auto engine = static_cast<v8kit::Engine*>(arg);
                delete engine;
            },
            engine
        );
    }

    engine->setData(std::make_shared<VMData>(id));

    auto vm = std::make_unique<VM>(id, engine, std::move(env));
    impl->vms_.emplace(id, std::move(vm));
    return impl->vms_[id].get();
}

void VMManager::destroyVM(VMID id) {
    if (!hasVM(id)) {
        return;
    }

    auto& vm = impl->vms_[id];

    _performDestroy(vm.get());

    impl->vms_.erase(id);
}

void VMManager::_performDestroy(VM* vm) {
    if (vm) {
        node::Stop(vm->nodeEnv_->env()); // send signal
    }
}

} // namespace bie