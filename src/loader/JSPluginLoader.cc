#include "JSPluginLoader.h"
#include "Entry.h"

#include "nlohmann/json.hpp"
#include "vm/VMData.h"
#include "vm/VMManager.h"

#include <fstream>
#include <iostream>

namespace bie {
namespace fs = std::filesystem;


JSPluginLoader::JSPluginLoader(endstone::Server& server) : PluginLoader(server) {}
JSPluginLoader::~JSPluginLoader() {
    auto entry = Entry::getInstance();
    if (entry) {
        auto& logger = entry->getLogger();
        logger.debug("JSPluginLoader is being destructed. Tearing down Node.js ecosystem...");

        auto& vmManager = entry->getVMManager();

        logger.debug("Shutting down libuv threads...");
        vmManager.shutdownUvLoopThread();

        logger.debug("Shutting down Node.js...");
        vmManager.shutdownNodeJs();
    }
}

inline std::string ReplaceStr(const std::string& str, std::string const& from, std::string const& to) {
    std::string result    = str;
    size_t      start_pos = 0;
    while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
        result.replace(start_pos, from.length(), to);
        start_pos += to.length(); // +1 for the advance
    }
    return result;
}

extern void init_plugin(VM* vm);

endstone::Plugin* JSPluginLoader::loadPlugin(std::string file) {
    auto  entry  = Entry::getInstance();
    auto& logger = entry->getLogger();

    file = ReplaceStr(file, "\\", "/"); // to unix path

    logger.debug("Loading plugin '{}'", file);

    auto&      manager = entry->getVMManager();
    auto       vm      = manager.createVM();
    auto       data    = vm->engine_->getData<VMData>();
    auto const id      = data->id;

    logger.debug("Allocating VM(ID: {}) for script '{}'", id, file);
    try {
        auto path = fs::path{file};

        data->fileName = path.filename().string();

        // TODO: setup binding api
        {
            v8kit::EngineScope lock{vm->engine_};
            init_plugin(vm);
        }

        // try load plugin
        {
            v8kit::EngineScope lock{vm->engine_};

            auto env     = vm->nodeEnv_->env();
            auto isolate = vm->nodeEnv_->isolate();

            node::SetProcessExitHandler(env, [id, isolate](node::Environment*, int exitCode) {
                Entry::getInstance()->getLogger().debug("Node.js process exit with code: {}, id: {}", exitCode, id);
                Entry::getInstance()->getVMManager().destroyVM(id);
            });

            v8::TryCatch vtry{isolate};
            // auto value = node::LoadEnvironment(env, fmt::format("require('module').Module.runMain('{}')", file));
            // if (value.IsEmpty() || vtry.HasCaught()) {
            //     v8::String::Utf8Value error(isolate, vtry.Exception());
            //     v8::String::Utf8Value stack(isolate, vtry.StackTrace(vm->nodeEnv_->context()).ToLocalChecked());
            //     Entry::getInstance()->getLogger().error("{}\n{}", *error, *stack);
            //     return nullptr;
            // }

            auto global = vm->engine_->globalThis();
            global.set(v8kit::String::newString("__pluginFilePath"), v8kit::String::newString(file));

            std::string bootstrap = R"(
                (async () => {
                    try {
                        // 标准化文件 URL
                        const fileUrl = require('url').pathToFileURL(globalThis.__pluginFilePath).href;

                        const mod = await import(fileUrl);

                        let PluginClass = mod.default;
                        if (!PluginClass) {
                            for (const key of Object.keys(mod)) {
                                if (typeof mod[key] === 'function' && mod[key].prototype instanceof globalThis.Plugin) {
                                    PluginClass = mod[key];
                                    break;
                                }
                            }
                        }
                        globalThis.__pluginClass = PluginClass;
                    } catch (err) {
                        globalThis.__pluginLoadError = err;
                    }
                })();
            )";

            auto value = node::LoadEnvironment(env, bootstrap);
            if (value.IsEmpty() || vtry.HasCaught()) {
                v8::String::Utf8Value error(isolate, vtry.Exception());
                logger.error("Failed to execute loader bootstrap:\n{}", *error);
                return nullptr;
            }

            auto       classKey = v8kit::String::newString("__pluginClass");
            auto       errorKey = v8kit::String::newString("__pluginLoadError");
            uv_loop_t* loop     = vm->nodeEnv_->event_loop();

            while (global.get(classKey).isUndefined() && global.get(errorKey).isUndefined()) {
                if (!uv_run(loop, UV_RUN_ONCE)) break;
            }

            if (!global.get(errorKey).isUndefined()) {
                v8::String::Utf8Value error(isolate, v8kit::ValueHelper::unwrap(global.get(errorKey)));
                logger.error("Plugin Module Load Error:\n{}", *error);
                return nullptr;
            }

            auto clsVal = global.get(classKey);
            if (clsVal.isUndefined() || !clsVal.isFunction()) {
                logger.error("Plugin must export a class inheriting from endstone::Plugin");
                return nullptr;
            }

            global.remove(classKey);
            global.remove(errorKey);
            global.remove(v8kit::String::newString("__pluginFilePath"));

            auto jsPluginInstance = clsVal.asFunction().callAsConstructor();
            if (!jsPluginInstance.isObject()) {
                logger.error("Failed to instantiate JS Plugin");
                return nullptr;
            }
            auto pluginThis = jsPluginInstance.asObject();

            auto payload = vm->engine_->getInstancePayload(pluginThis);
            if (!payload) return nullptr;

            data->pluginInst.reset(pluginThis); // 持久化 JS This 避免析构

            logger.debug("Loaded plugin {} with id {}", file, id);
            auto plugin = payload->unwrap<endstone::Plugin>();

            return plugin;
        }
    } catch (std::exception const& exception) {
        logger.error("Failed to load plugin {} with error: {}", file, exception.what());
    } catch (...) {
        logger.error("Failed to load plugin {} with unknown error", file);
    }
    manager.destroyVM(id);
    return nullptr;
}

std::vector<std::string> JSPluginLoader::getPluginFileFilters() const { return {".js"}; }

std::vector<std::string> JSPluginLoader::searchPlugins(std::filesystem::path const& directory) {
    std::vector<std::string> plugins;
    if (!fs::exists(directory)) {
        return plugins;
    }

    auto& logger = Entry::getInstance()->getLogger();
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_directory()) {
            continue;
        }

        fs::path package = entry.path() / "package.json";
        if (!fs::exists(package)) {
            continue;
        }

        auto main = extraMainScript(package);
        if (!main) {
            logger.error("No main script found in package.json for {}", entry.path().string());
            continue;
        }

        auto mainScript = entry.path() / main.value();
        if (!fs::exists(mainScript)) {
            logger.error("Main script {} not found in {}", main.value(), entry.path().string());
            continue;
        }

        plugins.push_back(mainScript.string());
    }

    return plugins;
}

std::optional<std::string> JSPluginLoader::extraMainScript(std::filesystem::path const& package) {
    std::ifstream ifs{package};
    if (!ifs.is_open()) {
        return std::nullopt;
    }
    try {
        auto json = nlohmann::json::parse(ifs);
        ifs.close();
        if (json.contains("main")) {
            return json["main"].get<std::string>();
        }
        return std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace bie