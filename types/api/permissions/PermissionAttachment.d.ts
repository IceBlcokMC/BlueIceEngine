/// <reference path="../../index.d.ts"/>

declare type PermissionRemovedExecutor = (
    attachment: PermissionAttachment
) => void;

/**
 * @hideconstructor
 * 该附件的详细信息，存储在Permissible对象中
 */
declare class PermissionAttachment {
    toString(): "<PermissionAttachment>" | string;

    /**
     * 获取这个附件的责任人插件
     *
     * @returns 负责此权限附件的责任人插件
     */
    getPlugin(): Plugin;

    /**
     * 设置一个在附件被移除时要调用的执行器。可以为空。
     *
     * @param callback 在移除此附件时要调用的执行器
     */
    setRemovalCallback(callback: PermissionRemovedExecutor): void;

    /**
     * 获取上一次设置为在附件被移除时要调用的执行器。可以为空。
     *
     * @return 上次设置为在附件被移除时要调用的执行器
     */
    getRemovalCallback(): PermissionRemovedExecutor;

    /**
     * 获取这个附件所附加到的Permissible对象
     *
     * @returns 包含此附件的Permissible对象
     */
    getPermissible(): Permissible;

    /**
     * 获取包含此附件中所有设置权限和值的副本。这个映射可以修改，但不会影响附件，因为它是副本。
     *
     * @return 包含该附件中所有设置权限和值的副本
     */
    getPermissions(): { [key: string]: boolean };

    /**
     * 设置指定权限的值，并通过其完全限定名称
     *
     * @param name 权限的名称
     * @param value 新权限的值
     */
    setPermission(name: string, value: boolean): void;

    /**
     * 设置指定权限的值
     *
     * @param perm 要设置的权限
     * @param value 新权限的值
     */
    setPermission(perm: Permission, value: boolean): void;

    /**
     * 从这个附件中移除指定权限。如果权限不存在于此附件，则不会发生任何操作。
     *
     * @param name 权限的名称要移除
     */
    unsetPermission(name: string): void;

    /**
     * 从这个附件中移除指定权限。如果权限不存在于此附件，则不会发生任何操作。
     *
     * @param perm 要移除的权限
     */
    unsetPermission(perm: Permission): void;

    /**
     * 将此附件从其注册的Permissible中删除
     *
     * @return 如果成功删除可允许，返回true；否则返回false
     */
    remove(): void;
}
