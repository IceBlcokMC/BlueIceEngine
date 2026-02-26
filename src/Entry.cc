#include "Entry.h"
#include "vm/VMManager.h"

#include "endstone/plugin/plugin_description.h"
#include "loader/JSPluginLoader.h"

#include <memory>
#include <thread>

#ifdef _MSC_VER
#include <debugapi.h>
#endif

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
    endstone::PluginDescription description = PluginDescriptionImpl{}.build("blue_ice_engine", "0.0.0");

    std::unique_ptr<VMManager> vmManager;
};

Entry::Entry() : impl(std::make_unique<Impl>()) {}
Entry::~Entry() { getInstance() = nullptr; }

void Entry::onLoad() {
    impl->vmManager = std::make_unique<VMManager>();

    auto& logger = getLogger();
    logger.setLevel(endstone::Logger::Debug);

#ifdef _MSC_VER
    logger.info("Waiting for VC debugger attach...");
    while (!IsDebuggerPresent()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
#endif

    logger.debug("Initializing Node.js");
    if (!impl->vmManager->initNodeJs()) {
        throw std::runtime_error("Failed to initialize Node.js");
    }

    logger.debug("Registering JS plugin loader");
    auto& server  = getServer();
    auto& manager = server.getPluginManager();
    manager.registerLoader(std::make_unique<JSPluginLoader>(server));

    logger.debug("Searching and attempting to load JS plugin...");
    manager.loadPlugins(JSPluginLoader::searchPlugins(std::filesystem::current_path() / "plugins"));
}

void Entry::onEnable() {
    // init libuv loop thread
    impl->vmManager->initUvLoopThread();
}

void Entry::onDisable() {
    impl->vmManager->shutdownUvLoopThread();
    // impl->vmManager->shutdownNodeJs();
    // impl->vmManager.reset();
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