#pragma once
#include <endstone/plugin/plugin_loader.h>
#include <string>
#include <vector>

namespace endstone {
class Server;
}

namespace jse {

class JavaScriptPluginLoader : public endstone::PluginLoader {
public:
    explicit JavaScriptPluginLoader(endstone::Server& server);

    [[nodiscard]] endstone::Plugin* loadPlugin(std::string file) override;

    [[nodiscard]] std::vector<std::string> getPluginFileFilters() const override;

    [[nodiscard]] std::vector<endstone::Plugin*> loadPlugins(std::string) override { return {}; }

public:
    [[nodiscard]] static std::vector<std::string> filterJsPlugins(const std::filesystem::path& directory);
};

} // namespace jse