/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 */
declare class Permission {
    toString(): "<Permission>" | string;

    /**
     * 返回此权限的唯一完全限定名称
     *
     * @return 完全限定名称
     */
    getName(): string;

    /**
     * 获取此权限的子节点。
     * 如果你修改了这个集合的任何形式，你需要调用 recalculatePermissibles() 重新计算所有 Permissibles
     *
     * 由于 JavaScript 的限制，您无法修改此集合。
     *
     * @return 子权限
     */
    getChildren(): { [k: string]: boolean };

    /**
     * 获取此权限的默认值。
     *
     * @return 默认值此权限.
     */
    getDefault(): Enums.PermissionDefault;

    /**
     * 设置此权限的默认值。
     *
     * 这将不会被保存到磁盘，且是一个临时操作，直到服务器重新加载权限。
     * 修改此默认值将导致所有包含此权限的 Permissibles 重新计算他们的权限
     *
     * @param value 新的默认值要设置
     */
    setDefault(value: Enums.PermissionDefault): void;

    /**
     * 获取此权限的简短描述，可能为空
     *
     * @return 简短描述此权限
     */
    getDescription(): string;

    /**
     * 设置此权限的描述。
     *
     * 这将不会被保存到磁盘，且是一个临时操作，直到服务器重新加载权限。
     *
     * @param value 新的描述要设置
     */
    setDescription(value: string): void;

    /**
     * 获取包含此权限的所有 Permissible 的集合。
     * 不能修改此集合。
     *
     * @return 包含具有此权限的所有 Permissible 的集合
     */
    getPermissibles(): Permissible[];

    /**
     * 根据权限的子节点重新计算所有被包含的权限。
     *
     * 应该在修改子节点后调用，自动在修改默认值后调用。
     */
    recalculatePermissibles(): void;

    /**
     * 将此权限添加到指定的父权限中。
     *
     * 如果父权限不存在，则将创建并注册它。
     *
     * @param name 父权限名称
     * @param value 设置此权限的值
     * @return 创建或加载的父权限
     */
    addParent(name: string, value: boolean): Permission;

    /**
     * 将此权限添加到指定的父权限中。
     *
     * @param perm 要注册的父权限
     * @param value 设置此权限的值
     */
    addParent(perm: Permission, value: boolean): void;

    // init(): any; // TODO: PluginManager
}
