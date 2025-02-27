#pragma once
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_description.h"

namespace jse {

struct JavaScriptPluginBuilder final : public endstone::detail::PluginDescriptionBuilder {
    std::string name;
    std::string version;

    endstone::PluginDescription build() {
        return endstone::detail::PluginDescriptionBuilder::build(this->name, this->version); // call base class method
    }
};

} // namespace jse