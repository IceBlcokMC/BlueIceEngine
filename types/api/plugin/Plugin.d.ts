/// <reference path="../../index.d.ts"/>

/**
 * 插件实例
 * @hideconstructor
 */
interface Plugin {
    toString(): "<Plugin>";

    /**
     * 返回这个插件的详细信息。
     *
     * @returns 插件的详细信息。
     */
    getDescription(): PluginDescription;

    /**
     * 模块加载后但在启用之前被调用。
     *
     * 当多个插件加载时，所有插件的onLoad()方法在任何onEnable()方法之前被调用。
     */
    onLoad(): void;

    /**
     * 插件启用后被调用。
     */
    onEnable(): void;

    /**
     * 插件禁用后被调用。
     */
    onDisable(): void;

    /**
     * 返回此服务器的日志记录器。返回的日志记录器自动为所有日志消息标记插件的名称。
     *
     * @return 关联于此插件的日志记录器
     */
    getLogger(): Logger;

    /**
     * 返回一个值，指示当前插件是否已启用。
     *
     * @return 如果此插件已启用，则返回true；否则返回false
     */
    isEnabled(): boolean;

    /**
     * 获取关联的PluginLoader负责此插件。
     *
     * @return 控制此插件的插件加载器
     */
    //getPluginLoader(): PluginLoader; // TODO: PluginLoader

    /**
     * 返回当前运行此插件的服务器实例。
     *
     * @return 正在运行此插件的服务器实例
     */
    getServer(): Server;

    /**
     * 返回插件的名称。
     *
     * 应该返回插件的原始名称，并应用于比较。
     *
     * @return 插件的名称
     */
    getName(): string;

    /**
     * 获取与给定名称相关的命令，这是特定于此插件的命令。
     *
     * @param name 名称或别名的命令
     * @return 如果找到，则返回插件命令；否则为null
     */
    // getCommand(): PluginCommand;

    /**
     * 返回插件数据文件的位置。文件可能尚未存在。
     *
     * @return 文件夹
     */
    getDataFolder(): string;

    // registerEvent(): any; // TODO: implement registerEvent
}
