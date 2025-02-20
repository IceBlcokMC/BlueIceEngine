#pragma once
#include <cstdint>
#include <endstone/detail/common.h>
#include <endstone/plugin/plugin.h>
#include <endstone/plugin/plugin_description.h>
#include <utility>


namespace jse {

class JsPluginDescriptionBuilder : public endstone::detail::PluginDescriptionBuilder {
public:
    JsPluginDescriptionBuilder() = default;
};

class JavaScriptPlugin : public endstone::Plugin {
public:
    JavaScriptPlugin(uint64_t engineId, endstone::PluginDescription description)
    : engineId_(engineId),
      description_(std::move(description)) {}
    ~JavaScriptPlugin() override;

public:
    void onLoad() override;
    void onEnable() override;
    void onDisable() override;

    [[nodiscard]] const endstone::PluginDescription& getDescription() const override;

    bool
    onCommand(endstone::CommandSender& sender, const endstone::Command& command, const std::vector<std::string>& args)
        override;

private:
    uint64_t                    engineId_;
    endstone::PluginDescription description_;
};


} // namespace jse
