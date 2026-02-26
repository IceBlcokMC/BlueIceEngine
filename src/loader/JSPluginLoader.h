#pragma once
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/server.h"

#include <string>
#include <vector>

namespace bie {


class JSPluginLoader : public endstone::PluginLoader {
public:
    explicit JSPluginLoader(endstone::Server& server);

    endstone::Plugin* loadPlugin(std::string file) override;

    std::vector<std::string> getPluginFileFilters() const override;

    std::vector<endstone::Plugin*> loadPlugins(std::string directory) override;

    static std::vector<std::string> searchPlugins(std::filesystem::path const& directory);

    static std::optional<std::string> extraMainScript(std::filesystem::path const& package);
};


} // namespace bie