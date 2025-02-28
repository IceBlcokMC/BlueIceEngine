#include "UsingCppTypes.h"
#include "V8Backend.hpp"
#include "endstone/command/command_executor.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin_load_order.h"
#include "puerts_impl/EnumImpl.h"

UsingCppEnum(endstone::PluginLoadOrder);
UsingCppEnum(endstone::PermissionDefault);

struct AutoBinding {
    AutoBinding() {
        DefineEnum(endstone::PluginLoadOrder)
            .Variable("PostWorld", endstone::PluginLoadOrder::PostWorld)
            .Variable("Startup", endstone::PluginLoadOrder::Startup)
            .Register();
        DefineEnum(endstone::PermissionDefault)
            .Variable("True", endstone::PermissionDefault::True)
            .Variable("False", endstone::PermissionDefault::False)
            .Variable("Operator", endstone::PermissionDefault::Operator)
            .Variable("NotOperator", endstone::PermissionDefault::NotOperator)
            .Register();

        puerts::DefineClass<endstone::CommandExecutor>().Register();

        puerts::DefineClass<endstone::Plugin>()
            .Extends<endstone::CommandExecutor>()
            // .Method("getDescription", &endstone::Plugin::getDescription)
            .Method("onLoad", MakeFunction(&endstone::Plugin::onLoad))
            .Method("onEnable", MakeFunction(&endstone::Plugin::onEnable))
            .Method("onDisable", MakeFunction(&endstone::Plugin::onDisable))
            .Method("getName", MakeFunction(&endstone::Plugin::getName))
            .Register();
    }
};
AutoBinding __AutoBinding__;
