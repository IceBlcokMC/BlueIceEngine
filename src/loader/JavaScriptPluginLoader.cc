#include "loader/JavaScriptPluginLoader.h"
#include "Entry.h"
#include "endstone/plugin/plugin.h"
#include "loader/JavaScriptPlugin.h"
#include "manager/NodeManager.h"
#include "uv.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include "v8_utils/V8Scope.h"
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace jse {

JavaScriptPluginLoader::JavaScriptPluginLoader(endstone::Server& server) : PluginLoader(server) {}
std::vector<std::string> JavaScriptPluginLoader::getPluginFileFilters() const { return {".js"}; }

endstone::Plugin* JavaScriptPluginLoader::loadPlugin(std::string file) {
    auto& manager = NodeManager::getInstance();
    auto  wrapper = manager.newScriptEngine();

    EngineID id = wrapper->mID;

    try {
        auto path            = std::filesystem::path(file);
        wrapper->mEntryPoint = path.filename().string();

        std::filesystem::path package = path.parent_path() / "package.json";
        if (NodeManager::packageHasDependency(package)
            && !std::filesystem::exists(path.parent_path() / "node_modules")) {
            Entry::getInstance()->getLogger().info("Installing dependencies for plugin: {}", path.filename().string());
            manager.NpmInstall(path.parent_path().string());
        }

        bool const esm = NodeManager::packageIsEsm(package);
        if (!NodeManager::loadFile(wrapper, file, esm)) {
            Entry::getInstance()->getLogger().error("Failed to load plugin: {}", file);
            manager.destroyEngine(wrapper->mID);
            return nullptr;
        }

        {
            EnterV8Scope scope{wrapper};

            if (esm) {
                size_t max   = 12; // 一般情况下, ESM 模块插件在第4个事件循环就会执行全局完全局代码
                size_t count = 0;
                while (count < max && !wrapper->mPluginDescriptionBuilder) {
                    count++;
                    uv_run(wrapper->mEnvSetup->event_loop(), UV_RUN_ONCE);
                }
                Entry::getInstance()->getLogger().debug("A total of {} event loops are executed", count);
                if (count == max && !wrapper->mPluginDescriptionBuilder) {
                    Entry::getInstance()->getLogger().warning("Failed to get plugin registration data, and the plugin "
                                                              "may not have called JSE.registerPlugin()");
                }
            }

            auto& plugin = wrapper->mPluginInstance;
            if (wrapper->mPluginDescriptionBuilder) {
                plugin = new JavaScriptPlugin(wrapper->mID, wrapper->mPluginDescriptionBuilder->build());
                wrapper->mPluginDescriptionBuilder.reset(); // 释放资源
            }
            return plugin;
        }
    } catch (std::exception& e) {
        Entry::getInstance()->getLogger().error("Failed to load plugin: {}", file);
        Entry::getInstance()->getLogger().error("Unknown error: {}", e.what());
    } catch (...) {
        Entry::getInstance()->getLogger().error("Failed to load plugin: {}", file);
        Entry::getInstance()->getLogger().error("Unknown error");
    }

    manager.destroyEngine(id);
    return nullptr;
}


std::vector<std::string> JavaScriptPluginLoader::filterPlugins(const std::filesystem::path& directory) {
    std::vector<std::string> plugins;
    if (!std::filesystem::exists(directory)) {
        return plugins;
    }

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (!entry.is_directory()) {
            continue;
        }

        std::filesystem::path package = entry.path() / "package.json";
        if (!std::filesystem::exists(package)) {
            continue;
        }

        auto main = NodeManager::findMainScript(package);
        if (!main) {
            continue;
        }

        plugins.push_back((entry.path() / *main).string());
    }

    return plugins;
}


} // namespace jse
