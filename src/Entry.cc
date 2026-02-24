#include "Entry.h"
#include "vm/VMManager.h"

#include "endstone/plugin/plugin_description.h"

#include <memory>


namespace bie {

class PluginDescriptionImpl : public endstone::detail::PluginDescriptionBuilder {
public:
    PluginDescriptionImpl() {
        description  = "A JavaScript runtime for endstone";
        contributors = {"BlueIceMc/BlueIceEngine all contributors"};
        website      = "https://github.com/IceBlcokMC/BlueIceEngine";
    }
};

struct Entry::Impl {
    endstone::PluginDescription description = PluginDescriptionImpl{}.build("blue_ice_engine", "");

    std::unique_ptr<VMManager> vmManager;
};

Entry::Entry() : impl(std::make_unique<Impl>()) {}
Entry::~Entry() { getInstance() = nullptr; }

void Entry::onLoad() {
    impl->vmManager = std::make_unique<VMManager>();

    if (!impl->vmManager->initNodeJs()) {
        throw std::runtime_error("Failed to initialize Node.js");
    }

    // TODO: load plugins
}

void Entry::onEnable() {
    // init libuv loop thread
    impl->vmManager->initUvLoopThread();
}

void Entry::onDisable() {
    impl->vmManager->shutdownUvLoopThread();
    impl->vmManager->shutdownNodeJs();
    impl->vmManager.reset();
}

endstone::PluginDescription const& Entry::getDescription() const { return impl->description; }

VMManager& Entry::getVMManager() { return *impl->vmManager; }

Entry*& Entry::getInstance() {
    static Entry* instance = nullptr; // init_endstone_plugin will set this
    return instance;
}

} // namespace bie

extern "C" [[maybe_unused]] ENDSTONE_EXPORT endstone::Plugin* init_endstone_plugin() {
    bie::Entry::getInstance() = new bie::Entry();
    return bie::Entry::getInstance();
}