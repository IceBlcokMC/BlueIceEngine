/// <reference path="../index.d.ts"/>
/**
 * @hideconstructor
 * 表示一个服务器实现。
 */
declare class Server {
    toString(): "<Server>" | string;

    /**
     * 获取这个服务器实现的名称。
     *
     * @return 这个服务器实现的名称
     */
    getName(): string;

    /**
     * 获取这个服务器实现的版本字符串。
     *
     * @return 这个服务器实现的版本
     */
    getVersion(): string;

    /**
     * 获取这个服务器正在运行的Minecraft版本。
     *
     * @return Minecraft版本
     */
    getMinecraftVersion(): string;

    /**
     * 返回与这个服务器实例关联的主日志记录器。
     *
     * @return 与这个服务器关联的日志记录器
     */
    getLogger(): Logger;

    /**
     * 获取当前用于服务器的语言界面。
     *
     * @return 用于服务器翻译的语言界面。
     */
    // getLanguage(): any;

    /**
     * 获取与插件交互的插件管理器。
     *
     * @return 用于这个服务器实例的插件管理器
     */
    // getPluginManager(): any;

    /**
     * 获取具有给定名称或别名的PluginCommand。
     *
     * @param name 命令的名称或别名
     * @return 如果找到，则返回一个PluginCommand；否则，返回null
     */
    // getPluginCommand(): any;

    /**
     * 获取这个服务器的CommandSender。
     *
     * @return 控制台命令发送器
     */
    // getCommandSender(): any;

    /**
     * 在这个服务器上派发命令，并如果找到，则执行它。
     *
     * @param sender 看似命令发送者的对象
     * @param command_line 命令及其参数。
     * @return 如果执行成功，返回true；否则，返回false
     */
    dispatchCommand(sender: CommandSender, command_line: string): any;

    /**
     * 获取用于管理调度事件的调度器。
     *
     * @return 用于这个服务器的调度服务
     */
    // getScheduler(): any;

    /**
     * 获取服务器 level.
     *
     * @return 服务器 level
     */
    getLevel(): Level;

    /**
     * 获取当前在线的玩家。
     *
     * @returns 当前在线的玩家
     */
    getOnlinePlayers(): Player[];

    /**
     * 获取可登录此服务器的最大玩家数量。
     *
     * @return 此服务器允许的玩家数量
     */
    getMaxPlayers(): number;

    /**
     * 设置允许同时登录的最大玩家数量。
     *
     * @param max_players 最大并发玩家数量
     */
    setMaxPlayers(max_players: number): void;

    /**
     * 根据给定的通用唯一识别码（UUID）获取玩家信息。
     *
     * @param uuid 要检索的玩家的通用唯一识别码（UUID）
     * @return 如果找到，则返回一个玩家对象；否则返回 null
     */
    getPlayer(uuid: UUID): Player;

    /**
     * 获取服务器是否处于在线模式。
     *
     * @return 如果服务器对客户端进行身份验证，则返回true；否则返回false
     */
    getOnlineMode(): boolean;

    /**
     * 根据给定的准确名称（不区分大小写）获取玩家。
     *
     * @param name 要检索的玩家的准确名称
     * @return 如果找到则返回一个玩家对象，否则返回 null
     */
    getPlayer(name: string): Player;

    /**
     * 关闭服务器，停止所有运行。
     */
    shutdown(): void;

    /**
     * 重新加载服务器配置、功能、脚本和插件。
     */
    reload(): void;

    /**
     * 仅重新加载服务器的 Minecraft 数据。
     *
     * 这包括所有行为包中的函数和脚本文件。
     */
    reloadData(): void;

    /**
     * 向具有指定权限名称的每个用户广播特定消息。
     *
     * @param message 要广播的消息
     * @param permission 接收广播的权限主体必须具备的所需权限
     */
    broadcast(message: Message, permission: string): void;

    /**
     * 向拥有“endstone.broadcast.user”权限的每位用户广播指定消息。
     *
     * @param message 消息内容
     */
    broadcastMessage(message: Message): void;

    /**
     * 将当前线程与预期的主服务器线程进行比对
     *
     * @return 如果当前线程与预期的主线程匹配，则返回true，否则返回false
     */
    isPrimaryThread(): boolean;

    /**
     * 获取由服务器控制的主计分板。
     * <p>
     * 此计分板由服务器保存，会受到“/scoreboard”命令的影响，并且是默认向玩家显示的计分板。只有在关卡加载完成后，此计分板才会存在。
     * </p>
     * @return 默认的服务器计分板
     */
    // getScoreboard(): any;

    /**
     * 创建一个由服务器跟踪的新计分板。
     * <p>
     * 此计分板不会由服务器保存，也不受“/scoreboard”命令的影响。
     * </p>
     * @return 新创建的计分板
     */
    // createScoreboard(): any;

    /**
     * 获取当前每刻的毫秒数（MSPT）。
     *
     * @return 当前每刻的平均毫秒数。
     */
    getCurrentMillisecondsPerTick(): number;

    /**
     * 获取平均每刻的毫秒数（MSPT）。
     *
     * @return 平均每刻的毫秒数。
     */
    getAverageMillisecondsPerTick(): number;

    /**
     * 获取当前每秒刻数（TPS）。
     *
     * @return 当前每秒刻数
     */
    getCurrentTicksPerSecond(): number;

    /**
     * 获取平均每秒刻数（TPS）。
     *
     * @return 平均每秒刻数
     */
    getAverageTicksPerSecond(): number;

    /**
     * 获取服务器当前的刻使用率。
     *
     * @return 当前刻使用率的百分比。
     */
    getCurrentTickUsage(): number;

    /**
     * 获取服务器的平均刻使用率。
     *
     * @return 平均刻使用率的百分比。
     */
    getAverageTickUsage(): number;

    /**
     * 获取服务器的启动时间。
     *
     * @return 服务器的启动时间。
     */
    getStartTime(): number;

    /**
     * 创建一个用于向玩家展示的 boss 栏实例。进度默认为 1.0。
     *
     * @param title boss 栏的标题
     * @param color boss 栏的颜色
     * @param style boss 栏的样式
     * @return 创建的 boss 栏
     */
    // createBossBar(): any;

    /**
     * 创建一个向玩家展示的boss栏实例。进度默认为1.0。
     *
     * @param title boss栏的标题
     * @param color boss栏的颜色
     * @param style boss栏的样式
     * @param flags 要在boss栏上设置的标志列表
     * @return 创建的boss栏
     */
    // createBossBar(): any;

    /**
     * 为指定的方块类型创建一个新的方块数据实例，所有属性均初始化为默认值。
     *
     * @param type 方块类型
     * @return 新的数据实例
     */
    // createBlockData(): any;

    /**
     * 为指定的方块类型创建一个新的方块数据实例，除了 `data` 中提供的属性外，所有属性均初始化为默认值。
     *
     * @param type 方块类型
     * @param block_states 方块状态，例如 {"old_leaf_type":"birch", "persistent_bit":true}
     * @return 新的数据实例
     */
    // createBlockData(): any;

    /**
     * 获取玩家封禁列表。
     *
     * @return 玩家封禁列表
     */
    // getBanList(): any;

    /**
     * 获取IP封禁列表。
     *
     * @return IP封禁列表
     */
    // getIpBanList(): any;

    /**
     * 用于所有管理消息，例如操作员使用命令时的消息。
     */
    static get BroadcastChannelAdmin(): string;

    /**
     * 用于所有公告消息，比如通知用户有玩家加入。
     */
    static get BroadcastChannelUser(): string;
}
