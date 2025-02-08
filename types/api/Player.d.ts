/// <reference path="../index.d.ts"/>

/**
 * 玩家类
 * @hideconstructor
 */
declare class Player extends Mob {
    toString(): "<Player>" | string;

    /**
     * 返回此玩家的UUID
     *
     * @returns 玩家的UUID
     */
    getUniqueId(): UUID;

    /**
     * 返回此玩家的Xbox用户ID（XUID）
     *
     * @returns 玩家的Xbox用户ID
     */
    getXuid(): string;

    /**
     * 获取此玩家的套接字地址
     *
     * @returns 玩家的套接字地址
     */
    getAddress(): SocketAddress;

    /**
     * 发送玩家弹出消息
     *
     * @param message 要显示的消息
     */
    sendPopup(message: string): void;

    /**
     * 发送玩家提示消息
     *
     * @param message 要显示的消息
     */
    sendTip(message: string): void;

    /**
     * 发送玩家提示通知。
     *
     * @param title 提示通知的标题。
     * @param content 提示通知的内容。
     */
    sendToast(title: string, content: string): void;

    /**
     * 将玩家踢出并自定义踢出消息。
     *
     * @param message 踢出消息
     */
    kick(message: string): void;

    /**
     * 给玩家指定经验量。
     *
     * @param amount 给予的经验量
     */
    giveExp(amount: number): void;

    /**
     * 给玩家指定经验等级。
     *
     * @param amount 要给定或减少的经验等级量
     */
    giveExpLevels(amount: number): void;

    /**
     * 获取玩家当前经验进度，接近下一个等级。
     *
     * 这是一个百分比值。0.0表示“没有进度”，1.0表示“下一个级别”。
     *
     * @returns 当前经验点数
     */
    getExpProgress(): number;

    /**
     * 设置玩家当前经验进度，接近下一个等级。
     *
     * 这是一个百分比值。0.0表示“没有进度”，1.0表示“下一个级别”。
     *
     * @param progress 新的体验进度
     */
    setExpProgress(progress: number): void;

    /**
     * 获取玩家当前经验等级。
     *
     * @returns 当前的经验等级
     */
    getExpLevel(): number;

    /**
     * 设置玩家当前经验等级。
     *
     * @param level 新的经验等级
     */
    setExpLevel(level: number): void;

    /**
     * 获取玩家总经验点数。
     *
     * 这是当前收集的总经验点数，而不是客户端显示的当前经验点数。
     *
     * @returns 当前总经验点数
     */
    getTotalExp(): number;

    /**
     * 确定玩家是否允许通过跳跃键双击飞行。
     *
     * @return 是否允许玩家通过跳跃键双击飞行
     */
    getAllowFlight(): boolean;

    /**
     * 设置玩家是否允许通过跳跃键双击飞行。
     *
     * @param flight 是否允许飞行
     */
    setAllowFlight(flight: boolean): void;

    /**
     * 检查此玩家是否当前飞行或不是。
     *
     * @return 是否正在飞行，否则为false
     */
    isFlying(): boolean;

    /**
     * 开始或停止玩家飞行。
     *
     * @param value 是否飞行
     */
    setFlying(value: boolean): void;

    /**
     * 获取当前允许客户端飞行的速度。
     *
     * @return 允许的当前速度，默认为0.05。
     */
    getFlySpeed(): number;

    /**
     * 设置客户端飞行的速度。
     *
     * @param value 新的速度
     */
    setFlySpeed(value: number): void;

    /**
     * 获取当前允许客户端行走的速度。
     *
     * @return 允许的当前速度，默认为0.10。
     */
    getWalkSpeed(): number;

    /**
     * 设置客户端行走的速度。
     *
     * @param value 新的速度
     */
    setWalkSpeed(value: number): void;

    /**
     * 获取此玩家显示的计分板
     *
     * @returns 触发此事件时当前玩家看到的计分板
     */
    //getScoreboard():Scoreboard; //TODO Scoreboard

    /**
     * 设置玩家可见的计分板。
     *
     * @param scoreboard 玩家的新计分板
     */
    //setScoreboard(scordboard:Scoreboard):void;//TODO Scoreboard

    /**
     * 发送标题和子标题消息给玩家。如果它们为空字符串，显示将按默认时间更新。标题和子标题将以默认时长显示。
     *
     * @param title 标题文本
     * @param subtitle 子标题文本
     */
    sendTitle(title: string, subtitle: string): void;

    /**
     * 发送标题和子标题消息给玩家。如果它们为空字符串，显示将按默认时间更新。
     *
     * @param title 标题文本
     * @param subtitle 子标题文本
     * @param fade_in 持续时间（以 ticks 为单位），标题在显示时 fades入。默认值为 10。
     * @param stay 持续时间（以 ticks 为单位），标题在显示时保持显示。默认值为 70。
     * @param fade_out 持续时间（以 ticks 为单位），标题在隐藏时 fades出。默认值为 20。
     */
    sendTitle(
        title: string,
        subtitle: string,
        fade_in: number,
        stay: number,
        fade_out: number
    ): void;

    /**
     * 重置玩家显示的标题。这将清除显示的标题和子标题，并重置计时器到默认值。
     */
    resetTitle(): void;

    /**
     * 在目标位置生成粒子效果。
     *
     * @param name 粒子效果的名称
     * @param location 生成粒子的位置
     */
    // spawnParticle(name:string, location:Location); //TODO Location

    /**
     * 在目标位置生成粒子效果。
     *
     * @param name 粒子效果的名称
     * @param x 生成粒子的位置（x 轴）
     * @param y 生成粒子的位置（y 轴）
     * @param z 生成粒子的位置（z 轴）
     */
    // spawnParticle(name: string, x: number, y: number, z: number): void;

    /**
     * 在目标位置生成粒子效果。
     *
     * @param name 粒子效果的名称
     * @param location 生成粒子的位置
     * @param molang_variables_json 需要自定义 Molang 变量的 JSON 表达式
     */
    // spawnParticle(location: Location, z: number, molang_variables_json?: string): void;

    /**
     * 在目标位置生成粒子效果。
     *
     * @param name 粒子效果的名称
     * @param x 生成粒子的位置（x 轴）
     * @param y 生成粒子的位置（y 轴）
     * @param z 生成粒子的位置（z 轴）
     * @param molang_variables_json 需要自定义 Molang 变量的 JSON 表达式
     */
    // spawnParticle(name: string, x: number, y: number, z: number, molang_variables_json?: string): void;

    /**
     * 获取玩家的平均 ping
     *
     * @returns 玩家的 ping
     */
    getPing(): number;

    /**
     * 将命令列表发送到客户端。
     *
     * 通常在权限更改完成后，确保客户端有完整的命令列表。
     */
    updateCommands(): void;

    /**
     * 让玩家执行给定的命令
     *
     * @param command 要执行的命令
     * @returns 命令是否成功，否则为 false
     */
    performCommand(command: string): boolean;

    /**
     * 获取此玩家当前的游戏模式
     *
     * @return 当前游戏模式
     */
    getGameMode(): Enums.GameMode;

    /**
     * 设置此玩家的当前游戏模式
     *
     * @param mode 新的游戏模式
     */
    setGameMode(mode: Enums.GameMode): void;

    /**
     * 获取玩家的 inventory。
     *
     * @return 玩家的.inventory，这也包含盔甲槽。
     */
    //getInventory():PlayerInventory; //TODO PlayerInventory

    /**
     * 获取玩家的当前语言环境。
     *
     * @return 玩家的语言环境
     */
    getLocale(): string;

    /**
     * 获取玩家的当前设备的操作系统（OS）。
     *
     * @return 玩家的设备 OS
     */
    getDeviceOS(): string;

    /**
     * 获取玩家的当前设备 ID。
     *
     * @return 玩家的设备 ID
     */
    getDeviceId(): string;

    /**
     * 获取玩家的游戏版本
     *
     * @returns 玩家的游戏版本
     */
    getGameVersion(): string;

    /**
     * 获取玩家的皮肤
     *
     * @return 玩家的皮肤
     */
    getSkin(): Skin;

    /**
     * 将玩家转移到另一个服务器
     *
     * @param host 转移玩家到的服务器地址
     * @param port 转移玩家到的服务器端口
     */
    transfer(host: string, port: number): void;

    /**
     * 发送表单到玩家。
     *
     * @param form 表单要发送
     */
    sendForm(form: MessageForm | ActionForm): void;

    /**
     * 关闭当前打开的表单给玩家。
     */
    closeForm(): void;

    /**
     * 将包发送到玩家。
     *
     * @param packet 要发送的包
     */
    // sendPacket(packet:Packet); //TODO Packet
}
