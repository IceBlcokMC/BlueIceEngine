#include "UsingCppTypes.h"
#include "V8Backend.hpp"
#include "endstone/command/command_executor.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin_load_order.h"


struct AutoBinding {
    AutoBinding() {
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
