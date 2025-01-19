/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 */
declare class PermissionAttachmentInfo {
    toString(): "<PermissionAttachmentInfo>" | string;

    getPermissible(): Permissible;

    getPermission(): string;

    getAttachment(): PermissionAttachment;

    getValue(): boolean;
}
