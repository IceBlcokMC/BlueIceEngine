#include "endstone/plugin/plugin.h"
#include "endstone/command/command.h"
#include "endstone/command/command_sender.h"
#include "endstone/command/plugin_command.h"
#include "endstone/plugin/plugin_loader.h"

#include "pch.h"
#include "vm/VM.h"

#include <string>
#include <vector>


namespace bie {


class JSPlugin : public endstone::Plugin, public v8kit::enable_trampoline {
public:
    JSPlugin() = default;

    void onLoad() override { V8KIT_OVERRIDE(void, Plugin, "onLoad", onLoad); }

    void onEnable() override { V8KIT_OVERRIDE(void, Plugin, "onEnable", onEnable); }

    void onDisable() override { V8KIT_OVERRIDE(void, Plugin, "onDisable", onDisable); }

    bool onCommand(
        endstone::CommandSender&        sender,
        endstone::Command const&        command,
        std::vector<std::string> const& args
    ) override {
        // v8kit unsupported ref stl container type (e.g. std::vector)
        V8KIT_OVERRIDE(bool, Plugin, "onCommand", onCommand, std::ref(sender), std::ref(command), args);
    }

    endstone::PluginDescription const& getDescription() const override {
        V8KIT_OVERRIDE_PURE(endstone::PluginDescription const&, Plugin, "getDescription", getDescription);
    }
};


void init_plugin(VM* vm) {
    using namespace v8kit::binding;

    static auto kPluginLoadOrder = defEnum<endstone::PluginLoadOrder>("PluginLoadOrder")
                                       .value("Startup", endstone::PluginLoadOrder::Startup)
                                       .value("PostWorld", endstone::PluginLoadOrder::PostWorld)
                                       .build();

    static auto kPluginLoader = defClass<endstone::PluginLoader>("PluginLoader")
                                    .ctor(nullptr)
                                    // TODO: bind more api
                                    .build();

    static auto kPluginCommand = defClass<endstone::PluginCommand>("PluginCommand")
                                     .ctor(nullptr)
                                     // TODO: bind more api
                                     .build();

    static auto kPluginDescription =
        defClass<endstone::PluginDescription>("PluginDescription")
            .ctor<
                std::string,
                std::string,
                std::string,
                endstone::PluginLoadOrder,
                std::vector<std::string>,
                std::vector<std::string>,
                std::string,
                std::string,
                std::vector<std::string>,
                std::vector<std::string>,
                std::vector<std::string>,
                std::vector<std::string>,
                endstone::PermissionDefault,
                std::vector<endstone::Command>,
                std::vector<endstone::Permission>>()
            .prop_readonly("name", &endstone::PluginDescription::getName)
            .prop_readonly("version", &endstone::PluginDescription::getVersion)
            .prop_readonly("fullName", &endstone::PluginDescription::getFullName)
            .prop_readonly("apiVersion", &endstone::PluginDescription::getAPIVersion)
            .prop_readonly("description", &endstone::PluginDescription::getDescription)
            .prop_readonly("load", &endstone::PluginDescription::getLoad)
            .prop_readonly("authors", &endstone::PluginDescription::getAuthors)
            .prop_readonly("contributors", &endstone::PluginDescription::getContributors)
            .prop_readonly("website", &endstone::PluginDescription::getWebsite)
            .prop_readonly("prefix", &endstone::PluginDescription::getPrefix)
            .prop_readonly("provides", &endstone::PluginDescription::getProvides)
            .prop_readonly("depend", &endstone::PluginDescription::getDepend)
            .prop_readonly("softDepend", &endstone::PluginDescription::getSoftDepend)
            .prop_readonly("loadBefore", &endstone::PluginDescription::getLoadBefore)
            .prop_readonly("defaultPermission", &endstone::PluginDescription::getDefaultPermission)
            .prop_readonly("commands", &endstone::PluginDescription::getCommands)
            .prop_readonly("permissions", &endstone::PluginDescription::getPermissions)
            .build();

    static auto kPlugin = defClass<JSPlugin>("Plugin")
                              .ctor()
                              .implements<endstone::Plugin>()
                              .method("onLoad", &JSPlugin::onLoad)
                              .method("onEnable", &JSPlugin::onEnable)
                              .method("onDisable", &JSPlugin::onDisable)
                              .method("getDescription", &JSPlugin::getDescription, ReturnValuePolicy::kReference)
                              .method("onCommand", &JSPlugin::onCommand) // remove?
                              .prop_readonly("logger", &JSPlugin::getLogger, ReturnValuePolicy::kReference)
                              .prop_readonly("pluginLoader", &JSPlugin::getPluginLoader, ReturnValuePolicy::kReference)
                              .prop_readonly("server", &JSPlugin::getServer, ReturnValuePolicy::kReference)
                              .prop_readonly("isEnabled", &JSPlugin::isEnabled)
                              .prop_readonly("name", &JSPlugin::getName)
                              .method("getCommand", &JSPlugin::getCommand, ReturnValuePolicy::kReference)
                              .prop_readonly("dataFolder", &JSPlugin::getDataFolder)
                              .build();


    auto engine = vm->engine_.get();
    engine->registerEnum(kPluginLoadOrder);
    engine->registerClass(kPluginLoader);
    engine->registerClass(kPluginCommand);
    engine->registerClass(kPluginDescription);
    engine->registerClass(kPlugin);
}


} // namespace bie
