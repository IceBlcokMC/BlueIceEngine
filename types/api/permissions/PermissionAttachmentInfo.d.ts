/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 *  该附件的详细信息，存储在Permissible对象中，并指定了提供这个权限的权限附件。
 */
declare class PermissionAttachmentInfo {
    toString(): "<PermissionAttachmentInfo>" | string;

    /**
     * 获取这个权限所附加到的Permissible对象
     *
     * @returns 包含此权限的Permissible对象
     */
    getPermissible(): Permissible;

    /**
     * 获取正在设置的权限名称
     *
     * @return 设置的权限名称
     */
    getPermission(): string;

    /**
     * 获取提供此权限的附件。通常，这是父权限或默认权限的附件。
     *
     * @return 附件
     */
    getAttachment(): PermissionAttachment;

    /**
     * 获取该权限的值
     *
     * @return 权限的值
     */
    getValue(): boolean;
}
