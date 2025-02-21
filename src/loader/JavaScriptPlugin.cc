#include "loader/JavaScriptPlugin.h"
#include "Entry.h"
#include "manager/NodeManager.h"
#include "utils/Using.h"
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

void JavaScriptPlugin::onLoad() { auto engine = NodeManager::getInstance().getEngine(this->engineId_); }

void JavaScriptPlugin::onEnable() { auto engine = NodeManager::getInstance().getEngine(this->engineId_); }

void JavaScriptPlugin::onDisable() { auto engine = NodeManager::getInstance().getEngine(this->engineId_); }

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