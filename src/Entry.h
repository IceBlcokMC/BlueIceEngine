#pragma once
#include "endstone/plugin/plugin.h"
#include <string>

namespace jse {

class PluginDescriptionBuilderImpl : public endstone::detail::PluginDescriptionBuilder {
public:
    PluginDescriptionBuilderImpl() {
        description  = "JavaScript Engine";
        website      = "https://github.com/engsr6982/Js_Engine";
        contributors = {"https://github.com/IceBlcokMC/Js_Engine/graphs/contributors"};
    }
};

class Entry : public endstone::Plugin {
public:
    static Entry*& getInstance();

    void onLoad() override;

    void onEnable() override;

    void onDisable() override;

    [[nodiscard]] endstone::PluginDescription const& getDescription() const override;

private:
    PluginDescriptionBuilderImpl builder;
    endstone::PluginDescription  description_ = builder.build(
        "js_engine",
        fmt::format("{}.{}.{}", JSENGINE_VERSION_MAJOR, JSENGINE_VERSION_MINOR, JSENGINE_VERSION_PATCH)
    );
};


} // namespace jse