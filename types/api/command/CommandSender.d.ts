/// <reference path="../../index.d.ts"/>

/**
 * 命令发送者
 * @hideconstructor
 */
declare class CommandSender extends Permissible {
    toString(): "<CommandSender>" | string;

    /** 转成命令发送者 */
    asCommandSender(): CommandSender | undefined;

    /**
     * 该方法获取一个CommandSender作为Console。
     *
     * @return CommandSender，如果不是Console，则返回null
     */
    // asConsole(): ConsoleCommandSender // TODO: ConsoleAPI

    /**
     * 该方法获取一个CommandSender作为Actor。
     *
     * @return Player，如果不是Actor，则返回null
     */
    asActor(): Actor | undefined;

    /**
     * 该方法获取一个CommandSender作为Mob。
     */
    asMob(): Mob | undefined;

    /**
     * 该方法获取一个CommandSender作为Player。
     *
     * @return Player，如果不是Player，则返回null
     */
    asPlayer(): Player | undefined;

    /**
     * 将这个发送者的消息发送出去
     *
     * @param message 要显示的消息
     */
    sendMessage(...message: any[]): void;

    /**
     * 将这个发送者的错误消息发送出去
     *
     * @param message 错误消息要显示
     */
    sendErrorMessage(...message: any[]): void;

    /**
     * 返回运行该命令的服务器实例
     *
     * @return 服务器实例
     */
    getServer(): Server;

    /**
     * 获取此命令发送者的名称
     *
     * @return 发送者的名字
     */
    getName(): string;
}
