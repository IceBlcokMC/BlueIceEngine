#pragma warning(disable : 4996)
#include "manager/NodeManager.h"
#include "CppObjectMapper.h"
#include "Entry.h"
#include "ObjectMapper.h"
#include "manager/RegisterGlobalFunc.h"
#include "utils/StringUtils.h"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-isolate.h"
#include "v8-locker.h"
#include "v8_utils/V8Exception.h"
#include "v8_utils/V8Scope.h"
#include <endstone/scheduler/scheduler.h>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <node.h>
#include <thread>
#include <uv.h>


#ifdef _WIN32
#include <Windows.h>
#include <shellapi.h>
#else
#include <cstdio>
#endif


namespace jse {

#pragma warning(disable : 4267)
#ifdef _WIN32
#pragma comment(lib, "Shell32.lib")
std::string wstr2str(const std::wstring& ws) {
    auto        len = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), nullptr, 0, nullptr, nullptr);
    std::string res(len + 1, 0);
    WideCharToMultiByte(CP_ACP, 0, ws.c_str(), ws.size(), res.data(), len, nullptr, nullptr);
    return res;
}
std::vector<std::string> GetArgs() {
    int                      argc = 0;
    auto                     argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    std::vector<std::string> res(argc);
    for (int i = 0; i < argc; i++) {
        res[i] = wstr2str(argv[i]);
    }
    LocalFree(argv);
    return res;
}
#else
std::vector<std::string> GetArgs() {
    FILE*                    cmdline = fopen("/proc/self/cmdline", "rb");
    std::vector<std::string> res;
    std::string              t;
    int                      c = fgetc(cmdline);
    while (c != EOF) {
        if (c == 0) {
            if (!t.empty()) res.push_back(t);
            t.clear();
        } else {
            t += c;
        }
        c = fgetc(cmdline);
    }
    fclose(cmdline);
    return res;
}
#endif
#pragma warning(default : 4267)

NodeManager& NodeManager::getInstance() {
    static NodeManager instance;
    return instance;
}

void NodeManager::initNodeJs() {
    if (mIsInitialized) {
        return;
    }
    static auto args = GetArgs();
    mArgs            = args;

    char* cWorkingDir = args[0].data();
    uv_setup_args(1, &cWorkingDir);
    // cppgc::InitializeProcess();
    auto result = node::InitializeOncePerProcess(
        args,
        {node::ProcessInitializationFlags::kNoInitializeV8,
         node::ProcessInitializationFlags::kNoInitializeNodeV8Platform}
    );
    mExecArgs = result->exec_args();
    if (result->exit_code() != 0) {
        Entry::getInstance()->getLogger().critical("Failed to initialize Node.js: ");
        for (auto const& error : result->errors()) {
            Entry::getInstance()->getLogger().critical(error);
        }
        return;
    }
    mPlatform = node::MultiIsolatePlatform::Create(std::thread::hardware_concurrency());
    v8::V8::InitializePlatform(mPlatform.get());
    v8::V8::Initialize();
    mIsInitialized = true;
}

void NodeManager::shutdownNodeJs() {
    for (auto& [id, wrapper] : mEngines) {
        this->destroyEngine(id);
    }

    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    node::TearDownOncePerProcess();
}

void NodeManager::initUvLoopThread() {
    std::thread([this]() {
        while (this->mUvLoopThreadRunning) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            try {
                for (auto& [id, wrapper] : mEngines) {
                    if (!wrapper->isRunning_ || wrapper->isDestroying_) {
                        continue;
                    }

                    EnterV8Scope scope(wrapper.get());
                    try {
                        v8::TryCatch vtry{wrapper->isolate()};
                        uv_run(wrapper->commonEnvPtr_->event_loop(), UV_RUN_NOWAIT);
                        v8_exception::check(vtry);
                    } catch (v8_exception const& except) {
                        // TODO: handle v8 exception
                    };
                }
            } catch (...) {};
        }
    }).detach();
}
void NodeManager::shutdownUvLoopThread() { mUvLoopThreadRunning = false; }


bool NodeManager::hasEngine(EngineID id) const { return mEngines.contains(id); }


V8Engine* NodeManager::newScriptEngine() {
    static EngineID NextEngineID = 0;
    if (!mIsInitialized) {
        return nullptr;
    }
    EngineID id = NextEngineID++;

    std::vector<std::string>                      errors;
    std::unique_ptr<node::CommonEnvironmentSetup> envSetup = node::CommonEnvironmentSetup::Create(
        mPlatform.get(),
        &errors,
        mArgs,
        mExecArgs,
        node::EnvironmentFlags::Flags(
            node::EnvironmentFlags::kNoRegisterESMLoader | node::EnvironmentFlags::kNoCreateInspector
            | node::EnvironmentFlags::kOwnsProcessState
        )
    );
    if (!envSetup) {
        for (auto const& err : errors)
            Entry::getInstance()->getLogger().error("Faild to create environment setup: {}", err);
        return nullptr;
    }

    v8::Isolate* isolate = envSetup->isolate();

    v8::Locker         locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope    handleScope(isolate);

    auto               context = envSetup->context();
    v8::Context::Scope contextScope(context);

    auto ptr = std::make_unique<V8Engine>(id, std::move(envSetup));

    auto mapper = new puerts::FCppObjectMapper();
    mapper->Initialize(isolate, context);
    isolate->SetData(MAPPER_ISOLATE_DATA_POS, static_cast<puerts::ICppObjectMapper*>(mapper));

    ptr->cppMapper_ = mapper;

    RegisterGlobalFunc(ptr.get());

    mEngines.emplace(id, std::move(ptr));

    node::AddEnvironmentCleanupHook(
        isolate,
        [](void* arg) {
            auto id = reinterpret_cast<EngineID>(arg);
            Entry::getInstance()->getLogger().debug("[EnvironmentCleanupHook] Destroyed engine: {}", id);
        },
        (void*)id
    );

    return mEngines[id].get();
}

V8Engine* NodeManager::getEngine(EngineID id) {
    if (mEngines.contains(id)) {
        return mEngines[id].get();
    }
    return nullptr;
}


bool NodeManager::destroyEngine(EngineID id) {
    try {
        if (!hasEngine(id)) {
            return false;
        }

        auto& wrapper = mEngines[id];

        // Entry::getInstance()->getServer().getScheduler().cancelTask(wrapper.mUvLoopTask->getTaskId());
        wrapper->isRunning_    = false;
        wrapper->isDestroying_ = true;

        // uv_stop(wrapper.mEnvSetup->event_loop()); (事件循环会自动停止)

        node::Stop(wrapper->commonEnvPtr_->env(), node::StopFlags::kDoNotTerminateIsolate);

        // wrapper.mEngine->destroy(); // 销毁引擎 (EnvironmentCleanupHook 会自动销毁)

        mEngines.erase(id); // 删除引擎

        return true;
    } catch (...) {
        Entry ::getInstance()->getLogger().error("Failed to destroy engine: {}", id);
        return false;
    }
}


bool NodeManager::NpmInstall(std::string npmExecuteDir) {
    if (!mIsInitialized) {
        return false;
    }

    std::vector<std::string> errors;
    auto                     setup = node::CommonEnvironmentSetup::Create(
        mPlatform.get(),
        &errors,
        mArgs,
        mExecArgs,
        node::EnvironmentFlags::kOwnsProcessState
    );
    if (!setup) {
        for (const std::string& err : errors)
            Entry::getInstance()->getLogger().error("CommonEnvironmentSetup Error: {}", err.c_str());
        return false;
    }
    npmExecuteDir = ll::string_utils::replaceAll(npmExecuteDir, "\\", "/");

    v8::Isolate*       isolate = setup->isolate();
    node::Environment* env     = setup->env();

    v8::Locker         locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);
    v8::Context::Scope context_scope(setup->context());

    Entry::getInstance()->getLogger().debug("Running npm install in {}", npmExecuteDir);
    // clang-format off
    std::string compiler = R"(
        try {
            const path = require("path");
            const cwd = path.join(process.cwd());

            let node;
            if (process.platform === "win32") {
                node = path.join(cwd, "node.exe");
            } else {
                node = path.join(cwd, "node");
            }

            const execute_dir = path.join(")"+npmExecuteDir+R"(");
            const npm_cli = path.join(cwd, "plugins/js_engine/node_modules/npm/bin/npm-cli.js");
            require("child_process").execFileSync(node, [npm_cli, "install"], { cwd: execute_dir });

            console.log("npm install success");
        } catch (e) {
            console.error(`Failed to run npm install:\n${e.stack}`);
        }
    )";
    // clang-format on

    bool success = false;
    try {
        node::SetProcessExitHandler(env, [&](node::Environment* /* env_ */, int /* exit_code */) { node::Stop(env); });
        v8::MaybeLocal<v8::Value> loadValue = node::LoadEnvironment(env, compiler);
        if (loadValue.IsEmpty()) {
            throw std::runtime_error("Failed to load environment");
        }
        success = node::SpinEventLoop(env).FromMaybe(1) == 0;
    } catch (...) {
        Entry::getInstance()->getLogger().error("Failed to run npm install");
    }

    node::Stop(env);
    return success;
}


bool NodeManager::loadFile(V8Engine* wrapper, std::filesystem::path const& path, bool esm) {
    if (!wrapper) {
        return false;
    }

#if defined(WIN32) || defined(_WIN32)
    std::string dirname  = ll::string_utils::replaceAll(path.parent_path().string(), "\\", "\\\\");
    std::string filename = ll::string_utils::replaceAll(path.string(), "\\", "\\\\");
#elif defined(__linux__)
    string dirname  = path.parent_path().string();
    string filename = path.string();
#endif
    Entry::getInstance()->getLogger().debug("dirname: {}", dirname);
    Entry::getInstance()->getLogger().debug("filename: {}", filename);

    try {
        std::string loader;
        if (esm) {
            loader = fmt::format(
                R"(
                    import('url').then(url => {{
                        const moduleUrl = url.pathToFileURL('{1}').href;
                        import(moduleUrl).catch(error => {{
                            console.error('Failed to load ESM module:', error);
                            process.exit(1);
                        }});
                    }}).catch(error => {{
                        console.error('Failed to import url module:', error);
                        process.exit(1);
                    }});
                )",
                dirname,
                filename
            );
        } else {
            loader = fmt::format(
                R"(
                    const __Path = require("path");
                    const __PluginPath = __Path.join("{0}");
                    const __PluginNodeModulesPath = __Path.join(__PluginPath, "node_modules");

                    __dirname = __PluginPath;
                    __filename = "{1}";
                    (function ReplaeRequire() {{
                        const PublicModule = require('module').Module;
                        const OriginalResolveLookupPaths = PublicModule._resolveLookupPaths;
                        PublicModule._resolveLookupPaths = function (request, parent) {{
                            let result = OriginalResolveLookupPaths.call(this, request, parent);
                            if (Array.isArray(result)) {{
                                result.push(__PluginNodeModulesPath);
                                result.push(__PluginPath);
                            }}
                            return result;
                        }};
                        require = PublicModule.createRequire(__PluginPath);
                    }})();
                    require("{1}");
                )",
                dirname,
                filename
            );
        }

        auto* env     = wrapper->env();
        auto* isolate = wrapper->isolate();

        {
            EnterV8Scope enter{wrapper};
            v8::TryCatch vtry(isolate);
            node::SetProcessExitHandler(env, [id{wrapper->id_}, isolate](node::Environment*, int exit_code) {
                isolate->Exit();
                Entry::getInstance()->getLogger().debug("Node.js process exit with code: {}, id: {}", exit_code, id);
                if (exit_code == 0) NodeManager::getInstance().destroyEngine(id);
            });
            v8_exception::check(vtry);
        }

        {
            EnterV8Scope enter{wrapper};
            v8::TryCatch vtry(isolate);

            v8::MaybeLocal<v8::Value> loadValue = node::LoadEnvironment(env, loader);
            if (loadValue.IsEmpty()) {
                v8_exception::check(vtry);
                return false;
            }
            v8_exception::check(vtry);
        }

        wrapper->isRunning_ = true;
        return true;
    } catch (v8_exception const& exc) {
        EnterV8Scope enter{wrapper};
        Entry::getInstance()->getLogger().error("Uncaught v8_exception: {}\n{}", exc.message(), exc.stacktrace());
        return false;
    } catch (...) {
        return false;
    }
}


std::optional<std::string> NodeManager::readFileContent(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        Entry::getInstance()->getLogger().error("Cannot open file: {}", path.string());
        return std::nullopt;
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return std::move(content);
}


std::optional<std::string> NodeManager::findMainScript(const std::filesystem::path& packagePath) {
    try {
        if (!std::filesystem::exists(packagePath)) {
            return std::nullopt;
        }

        auto content = readFileContent(packagePath);
        if (!content) {
            return std::nullopt;
        }

        auto json = nlohmann::json::parse(*content);
        return json.value("main", "");
    } catch (...) {
        return std::nullopt;
    }
}

bool NodeManager::packageHasDependency(const std::filesystem::path& packagePath) {
    try {
        if (!std::filesystem::exists(packagePath)) {
            return false;
        }

        auto content = readFileContent(packagePath);
        if (!content) {
            return false;
        }

        auto json = nlohmann::json::parse(*content);
        return json.contains("dependencies");
    } catch (...) {
        return false;
    }
}

bool NodeManager::packageIsEsm(const std::filesystem::path& packagePath) {
    try {
        if (!std::filesystem::exists(packagePath)) {
            return false;
        }

        auto content = readFileContent(packagePath);
        if (!content) {
            return false;
        }

        auto json = nlohmann::json::parse(*content);
        return json.contains("type") && json.value("type", "") == "module";
    } catch (...) {
        return false;
    }
}


} // namespace jse