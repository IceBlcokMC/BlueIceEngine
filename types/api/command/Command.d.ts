/// <reference path="../../index.d.ts"/>

/**
 * 命令接口
 * @hideconstructor
 */
declare class Command {
    toString(): "<Command>" | string;

    /**
     * 执行命令并返回其成功状态。
     *
     * @param sender 指令的来源
     * @param args 命令传递给该指令的参数
     * @return 如果执行成功则返回true，否则返回false
     */
    execute(sender: CommandSender, args: string[]): boolean;

    /**
     * 获取此命令的名称
     *
     * @return 此命令的名称
     */
    getName(): string;

    /**
     * 设置此命令的名称。
     * 可以在注册命令之前使用。
     *
     * @param name 新命令名称
     */
    setName(name: string): void;

    /**
     * 获取此命令的简要描述
     *
     * @return 该命令的简要描述
     */
    getDescription(): string;

    /**
     * 设置此命令的简要描述。
     *
     * @param description 新命令描述
     */
    setDescription(description: string): void;

    /**
     * 返回此命令的别名列表
     *
     * @return 别名列表
     */
    getAliases(): string[];

    /**
     * 将注册指令设置为此命令的别名列表。
     *
     * @param aliases 要请求注册到该命令的别名列表
     */
    setAliases(...aliases: string[]): void;

    /**
     * 获取此命令的用法列表
     *
     * @return 用法列表
     */
    getUsages(): string[];

    /**
     * 设置此命令的用法列表。
     *
     * @param usages 用法列表
     */
    setUsages(...usages: string[]): void;

    /**
     * 获取用户执行此命令所需的权限名称列表。
     *
     * @return 用户执行此命令所需权限名称列表，如果没有权限则返回空列表
     */
    getPermissions(): string[];

    /**
     * 设置用户执行此命令所需的权限名称列表。
     *
     * @param permissions 用户执行此命令所需权限名称列表
     */
    setPermissions(...permissions: string[]): void;

    /**
     * 测试给定的CommandSender是否可以执行此命令。如果他们没有权限，则将被通知不能执行此操作
     *
     * @param target 要测试的用户
     * @return 如果他们能够使用它，则返回true，否则返回false
     */
    testPermission(target: CommandSender): boolean;

    /**
     * 测试给定的CommandSender是否可以执行此命令。不会发送错误信息到发送者。
     *
     * @param target 要测试的用户
     * @return 如果他们能够使用它，则返回true，否则返回false
     */
    testPermissionSilently(target: CommandSender): boolean;

    /**
     * 将此命令注册到CommandMap中。
     *
     * @param command_map 注册到该命令的CommandMap
     * @return 如果注册成功则返回true，否则返回false
     */
    // registerTo(): any; // TODO: CommandMap

    /**
     * 从CommandMap中取消注册此命令。
     *
     * @param command_map 注销从该命令的CommandMap
     * @return 如果取消注册成功则返回true，否则返回false
     */
    // unregisterFrom(): any; // TODO: CommandMap

    /**
     * 返回当前注册此命令的状态。
     *
     * @return 如果此命令正在注册，则返回true，否则返回false
     */
    isRegistered(): boolean;

    // asPluginCommand(): any; // TODO: PluginCommand
}
