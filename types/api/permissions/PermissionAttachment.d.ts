/// <reference path="../../index.d.ts"/>

declare type PermissionRemovedExecutor = (
    attachment: PermissionAttachment
) => void;

/**
 * @hideconstructor
 */
declare class PermissionAttachment {
    toString(): "<PermissionAttachment>" | string;

    getPlugin(): Plugin;

    setRemovalCallback(callback: PermissionRemovedExecutor): void;

    getRemovalCallback(): PermissionRemovedExecutor;

    getPermissible(): Permissible;

    getPermissions(): { [key: string]: boolean };

    setPermission(name: string, value: boolean): void;
    setPermission(perm: Permission, value: boolean): void;

    unsetPermission(name: string): void;
    unsetPermission(perm: Permission): void;

    remove(): void;
}
