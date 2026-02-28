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

    // 由于 endstone 的 API 设计默认是在 EndstoneServer::loadPlugins 中注册内置的 Py/CppPluginLoader
    // 且 BlueIceEngine 受 CppPluginLoader 加载，导致无法重入加载流程, 故这里只能手动加载，模拟 onLoad 阶段
    logger.debug("Searching and attempting to load JS plugin...");
    manager.loadPlugins(JSPluginLoader::searchPlugins(std::filesystem::current_path() / "plugins"));
}

void Entry::onEnable() {
    auto& logger = getLogger();
    logger.debug("Initializing libuv thread...");
    impl->vmManager->initUvLoopThread();
}

void Entry::onDisable() {
    auto& logger = getLogger();

    // https://github.com/EndstoneMC/endstone/pull/339
    // 由于 endstone 的插件卸载机制没有任何拓扑排序，此 PR 采用倒序修复了 endstone 先卸载插件、后卸载 loader 导致的崩溃
    // 但同时因为倒序卸载插件，导致这里不能同步阻塞加载，因为如果同步加载，会导致 JSPlugin* 提前 push 到 vector 内
    // 而引擎却在所有 js 插件后面，在服务器关闭时，就会出现引擎先卸载，JS插件后卸载，进而导致崩溃
    // 已知目前是倒序析构，上面的坑都无法避免，那么把 Node.js 清理塞到 ~JSPluginLoader 内部处理
    logger.debug("BlueIceEngine is disabling (VM is kept alive for JS plugins)...");
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