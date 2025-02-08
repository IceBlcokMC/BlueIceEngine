/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 * 可能可以成为服务器管理员并可分配权限的对象。
 */
declare class Permissible {
    toString(): "<Permissible>" | string;

    /**
     * @brief 检查此对象是否是服务器管理员
     *
     * @return true 如果这是服务器管理员，否则 false
     */
    isOp(): boolean;

    /**
     * @brief 设置此对象的管理员状态
     *
     * @param value 新的管理员值
     */
    setOp(value: boolean): void;

    /**
     * 检查此对象中是否存在指定权限的完全限定名称的覆盖
     *
     * @param name 权限名称
     * @return 如果权限设置，则为 true，否则为 false
     */
    isPermissionSet(name: string): boolean;

    /**
     * 检查此对象中是否存在指定权限的覆盖
     *
     * @param perm 权限检查
     * @return 如果权限设置，则为 true，否则为 false
     */
    isPermissionSet(perm: Permission): boolean;

    /**
     * 获取指定权限的值，如果已设置。如果没有设置此对象中的权限覆盖，则返回权限的默认值
     *
     * @param name 权限名称
     * @return 权限的值
     */
    hasPermission(name: string): boolean;

    /**
     * 获取指定权限的值，如果已设置。如果没有设置此对象中的权限覆盖，则返回权限的默认值
     *
     * @param perm 权限获取
     * @return 权限的值
     */
    hasPermission(perm: Permission): boolean;

    /**
     * 添加一个新的PermissionAttachment，并使用名称和值注册一个单个权限
     *
     * @param plugin 负责此附件的插件，不能为 null 或禁用
     * @param name 注册的权限名称
     * @param value 注册的权限值
     * @return 创建的 PermissionAttachment
     */
    // addAttachment(plugin: Plugin): any; // TODO: PermissionAttachment

    /**
     * 添加一个新的空 PermissionAttachment 到此对象
     *
     * @param plugin 负责此附件的插件，不能为 null 或禁用
     * @return 创建的 PermissionAttachment
     */
    // addAttachment(plugin: Plugin, name: string, value: boolean): any; // TODO: PermissionAttachment

    /**
     * 从这个对象中删除指定的 PermissionAttachment
     *
     * @param attachment 要删除的 Attachment
     * @return 如果成功删除了指定的附件，则返回 true，否则当它不是此对象的一部分时返回false
     */
    // removeAttachment(attachment: PermissionAttachment): any; // TODO: PermissionAttachment

    /**
     * 根据附件的变化值重新计算此对象的权限。这通常很少需要从插件中调用。
     */
    recalculatePermissions(): void;

    /**
     * 获取此对象当前有效的所有权限的集合
     *
     * @return 当前有效权限的集合
     */
    //getEffectivePermissions(): PermissionAttachmentInfo[] // TODO: PermissionAttachmentInfo

    /**
     * @brief 将 Permissible 转换为 CommandSender
     *
     * @return CommandSender，如果不是一个 CommandSender，则返回 nullptr
     */
    asCommandSender(): CommandSender | undefined;
}
