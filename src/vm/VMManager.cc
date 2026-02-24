#include "VMManager.h"
#include "Entry.h"
#include "vm/VM.h"

#include "node.h"
#include "uv.h"
#include "v8-context.h"
#include "v8-locker.h"
#include "v8kit/core/Engine.h"
#include "v8kit/core/EngineScope.h"
#include "v8kit/core/Exception.h"
#include "v8kit/core/Fwd.h"
#include "vm/VMData.h"


#include <atomic>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>


namespace bie {

struct VMManager::Impl {
    bool                                          isInited = false;
    std::vector<std::string>                      argv;
    std::vector<std::string>                      exec_args;
    std::unique_ptr<node::MultiIsolatePlatform>   platform;
    std::thread                                   uvThread;
    std::mutex                                    uvExitMutex;
    std::condition_variable                       uvExitCv;
    std::atomic<bool>                             isUvLoopRunning = false;
    bool                                          uvThreadExited  = false;
    VMID                                          nextVMID        = 0;
    std::unordered_map<VMID, std::unique_ptr<VM>> vms;
};

VMManager::VMManager() : impl(std::make_unique<Impl>()) {}

VMManager::~VMManager() = default;

bool VMManager::isNodeJsInitialized() const { return impl->isInited; }

bool VMManager::initNodeJs() {
    if (impl->isInited) return true;
    impl->isInited = true;

    auto  workingDir     = std::filesystem::current_path() / "bedrock_server.exe";
    char* charWorkingDir = (char*)workingDir.c_str();

    impl->argv.push_back(workingDir.string()); // cache argv

    uv_setup_args(1, &charWorkingDir); // init libuv

    auto result = node::InitializeOncePerProcess(
        impl->argv,
        {
            node::ProcessInitializationFlags::kNoInitializeV8,
            node::ProcessInitializationFlags::kNoInitializeNodeV8Platform,
        }
    );
    impl->exec_args = result->exec_args();

    if (result->exit_code() != 0) {
        Entry::getInstance()->getLogger().critical("Failed to initialize Node.js");
        for (auto& err : result->errors()) {
            Entry::getInstance()->getLogger().critical(err);
        }
        return false;
    }

    impl->platform = node::MultiIsolatePlatform::Create(std::thread::hardware_concurrency());

    v8::V8::InitializePlatform(impl->platform.get());
    v8::V8::Initialize();
    return true;
}

bool VMManager::shutdownNodeJs() {
    if (!impl->isInited) return true;

    for (auto& [id, vm] : impl->vms) {
        _performDestroy(vm.get());
    }
    impl->vms.clear();

    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    node::TearDownOncePerProcess();
    impl->isInited = false;
    return true;
}

void VMManager::initUvLoopThread() {
    bool sou = false;
    impl->isUvLoopRunning.compare_exchange_strong(sou, true);
    impl->uvThreadExited = false;

    impl->uvThread = std::thread([this]() {
        while (impl->isUvLoopRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            try {
                for (auto& [id, vm] : impl->vms) {
                    if (vm->engine_->isDestroying()) {
                        continue;
                    }
                    v8kit::EngineScope lock{vm->engine_.get()};
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
            std::lock_guard lk(impl->uvExitMutex);
            impl->uvThreadExited = true;
        }
        impl->uvExitCv.notify_one();
    });
}
void VMManager::shutdownUvLoopThread() {
    impl->isUvLoopRunning = false;

    if (impl->uvThread.joinable()) {
        std::unique_lock lk(impl->uvExitMutex);
        impl->uvExitCv.wait(lk, [this] { return impl->uvThreadExited; });
        lk.unlock();

        impl->uvThread.join();
    }
}

bool VMManager::hasVM(VMID id) const { return impl->vms.contains(id); }

VM* VMManager::createVM() {
    if (!impl->isInited) {
        return nullptr;
    }

    auto const id = impl->nextVMID++;

    std::vector<std::string> errors;
    auto                     env = node::CommonEnvironmentSetup::Create(
        impl->platform.get(),
        &errors,
        impl->argv,
        impl->exec_args,
        node::EnvironmentFlags::Flags(
            node::EnvironmentFlags::kNoCreateInspector // 禁用 inspector
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
        engine = std::make_unique<v8kit::Engine>(isolate, env->context());
    }

    engine->setData(std::make_shared<VMData>(id));

    auto vm = std::make_unique<VM>(id, std::move(engine), std::move(env));
    impl->vms.emplace(id, std::move(vm));
    return impl->vms[id].get();
}

void VMManager::destroyVM(VMID id) {
    if (!hasVM(id)) {
        return;
    }

    auto& vm = impl->vms[id];

    _performDestroy(vm.get());

    // destroy vm
    impl->vms.erase(id);
}

void VMManager::_performDestroy(VM* vm) {
    if (!vm) return;

    // destroy engine
    vm->engine_.reset();
    // send stop signal
    node::Stop(vm->nodeEnv_->env(), node::StopFlags::kDoNotTerminateIsolate);
    // destroy node env
    vm->nodeEnv_.reset();
}

} // namespace bie