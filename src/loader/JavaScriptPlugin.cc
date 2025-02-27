#include "loader/JavaScriptPlugin.h"
#include "Entry.h"
#include "manager/NodeManager.h"
#include "v8-exception.h"
#include "v8_utils/V8Exception.h"
#include "v8_utils/V8Scope.h"
#include <endstone/command/command.h>
#include <endstone/logger.h>
#include <iostream>


namespace jse {


JavaScriptPlugin::~JavaScriptPlugin() {
    NodeManager::getInstance().destroyEngine(this->engineId_);
#ifdef DEBUG
    std::cout << "JavaScriptPlugin::~JavaScriptPlugin()  Plugin: " << this->getName() << std::endl;
#endif
}

void JavaScriptPlugin::onLoad() {
    auto         engine = NodeManager::getInstance().getEngine(this->engineId_);
    EnterV8Scope enter{engine};
    v8::TryCatch vtry{engine->isolate()};
    if (!engine->onLoad_.IsEmpty()) {
        [[maybe_unused]] auto v =
            engine->onLoad_.Get(engine->isolate())->Call(engine->context(), engine->context()->Global(), 0, nullptr);
    }
    v8_exception::check(vtry);
}

void JavaScriptPlugin::onEnable() {
    auto         engine = NodeManager::getInstance().getEngine(this->engineId_);
    EnterV8Scope enter{engine};
    v8::TryCatch vtry{engine->isolate()};
    if (!engine->onEnable_.IsEmpty()) {
        [[maybe_unused]] auto v =
            engine->onEnable_.Get(engine->isolate())->Call(engine->context(), engine->context()->Global(), 0, nullptr);
    }
    v8_exception::check(vtry);
}

void JavaScriptPlugin::onDisable() {
    auto         engine = NodeManager::getInstance().getEngine(this->engineId_);
    EnterV8Scope enter{engine};
    v8::TryCatch vtry{engine->isolate()};
    if (!engine->onDisable_.IsEmpty()) {
        [[maybe_unused]] auto v =
            engine->onDisable_.Get(engine->isolate())->Call(engine->context(), engine->context()->Global(), 0, nullptr);
    }
    v8_exception::check(vtry);
}

bool JavaScriptPlugin::onCommand(
    endstone::CommandSender&        sender,
    const endstone::Command&        command,
    const std::vector<std::string>& args
) {
    auto engine = NodeManager::getInstance().getEngine(this->engineId_);

    return true;
}

endstone::PluginDescription const& JavaScriptPlugin::getDescription() const { return this->description_; }


} // namespace jse