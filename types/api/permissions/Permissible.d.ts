/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 */
declare class Permissible {
	toString(): "<Permissible>" | string;

    isOp(): boolean;

    setOp(value: boolean): void;

    isPermissionSet(name: string): boolean;

    isPermissionSet(perm: Permission): boolean;

    hasPermission(name: string): boolean;

    hasPermission(perm: Permission): boolean;

    // addAttachment(plugin: Plugin): any; // TODO: PermissionAttachment

    // addAttachment(plugin: Plugin, name: string, value: boolean): any; // TODO: PermissionAttachment

    // removeAttachment(attachment: PermissionAttachment): any; // TODO: PermissionAttachment

    recalculatePermissions(): void;

    //getEffectivePermissions(): PermissionAttachmentInfo[] // TODO: PermissionAttachmentInfo

    asCommandSender(): CommandSender | undefined;
}
