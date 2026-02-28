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

    // TODO: 这里的加载流程移到  onEnable 内处理
    // https://github.com/EndstoneMC/endstone/pull/339
    // 由于 endstone 的插件卸载机制没有任何拓扑排序，此 PR 采用倒序修复了 endstone 先卸载插件、后卸载 loader 导致的崩溃
    // 但同时因为倒序卸载插件，导致这里不能同步阻塞加载，因为如果同步加载，会导致 JSPlugin* 提前 push 到 vector 内
    // 而引擎却在所有 js 插件后面，在服务器关闭时，就会出现引擎先卸载，JS插件后卸载，进而导致崩溃
    // WIP: 此问题暂时无解，为了稳定性，目前先采用 onEnable 加载方案

    // 问题跟踪：endstone 的 depend 机制并不能解决依赖问题，目前这个 depend 机制尚不明确加载行为
    // 且 depend 似乎仅针对加载，查看实现不是拓扑排序，且卸载未作排序处理。

    // 由于 endstone 的 API 设计默认是在 EndstoneServer::loadPlugins 中注册内置的 Py/CppPluginLoader
    // 且 BlueIceEngine 受 CppPluginLoader 加载，导致无法重入加载流程, 故这里只能手动加载，模拟 onLoad 阶段
    // logger.debug("Searching and attempting to load JS plugin...");
    // manager.loadPlugins(JSPluginLoader::searchPlugins(std::filesystem::current_path() / "plugins"));
}

void Entry::onEnable() {
    auto& logger  = getLogger();
    auto& manager = getServer().getPluginManager();

    // TODO: 别听上面的，依旧无解
    // 虽然 PR　修复了致命的 CppPluginLoader 先卸载，引擎被释放 ~JSPluginLoader 的 vftable 跳转到已释放内存上，
    //  但是依旧无法解决引擎实际加载 VM 后，引擎比 JSPlugin 先死的问题，且目前的问题是引擎先 onDisable 了，但
    //  JSPluginLoader 依旧后析构！
    // 问题解决，解决方案是 ~JSPluginLoader 里不删VM了，但出现另一个问题，脚本的 onDisable 钩子未触发...
    //  调试发现 EndstoneServer::enablePlugins(PluginLoadOrder type) 是拷贝 onLoad 阶段加载的插件指针进行设置 enabled
    //  但 JSPlugin 是 onEnable 阶段加载的，导致插件不在要 enable 的vector里，跳过了enable所以不触发 onDisable
    logger.debug("Searching and attempting to load JS plugin...");
    manager.loadPlugins(JSPluginLoader::searchPlugins(std::filesystem::current_path() / "plugins"));

    // TODO: 迁移到主线程同步调度?
    // [20:14:12 ERROR]: PluginEnableEvent must be triggered synchronously from server thread.
    logger.debug("Initializing libuv thread...");
    impl->vmManager->initUvLoopThread();
}

void Entry::onDisable() {
    auto& logger = getLogger();

    logger.debug("Shutting down libuv threads...");
    impl->vmManager->shutdownUvLoopThread();

    logger.debug("Shutting down Node.js...");
    impl->vmManager->shutdownNodeJs();

    logger.debug("Cleaning up...");
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