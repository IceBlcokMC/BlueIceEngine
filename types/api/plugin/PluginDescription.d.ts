/// <reference path="../../index.d.ts"/>

/**
 * 插件加载器需要知道的基本信息，该基本信息用于表示插件的简要信息。
 * @hideconstructor
 */
declare class PluginDescription {
    toString(): "<PluginDescription>" | string;

    /**
     * 返回插件的名称。此名称是一个唯一的标识符，用于标识插件。
     *
     * @return 插件的名称
     */
    getName(): string;

    /**
     * 返回插件的版本号。
     *
     * @return 插件的版本号
     */
    getVersion(): string;

    /**
     * 返回插件的名称，包括版本号。
     *
     * @return 描述此插件和相应版本的简要名称
     */
    getFullName(): string;

    /**
     * 获取插件设计支持的API版本。
     *
     * @return 该插件设计支持的API版本
     */
    getAPIVersion(): string;

    /**
     * 获取提供给插件的功能描述。如果未指定，则返回空字符串。
     *
     * @return 插件提供的功能描述，或者如果未指定则为空字符串
     */
    getDescription(): string;

    /**
     * 返回插件启动时应该加载的阶段。
     *
     * @return 插件应该在启动时加载的阶段
     */
    getLoad(): Enums.PluginLoadOrder;

    /**
     * 获取插件作者列表。
     *
     * @return 一个不可变的插件作者列表
     */
    getAuthors(): string[];

    /**
     * 获取插件贡献者列表。
     *
     * @return 一个不可变的插件贡献者列表
     */
    getContributors(): string[];

    /**
     * 返回此插件或其作者的网站地址。
     *
     * @return 插件的网站地址，如果未指定则为空字符串
     */
    getWebsite(): string;

    /**
     * 返回前缀插件特定日志消息的日志前缀。如果未指定，则返回空字符串。
     */
    getPrefix(): string;

    /**
     * 获取提供给其他插件使用的插件API列表。这些可用于其他插件依赖于。
     *
     * @return 一个不可变的插件API列表，该列表用于提供给其他插件使用的插件API
     */
    getProvides(): string[];

    /**
     * 获取需要此插件的其他插件列表。
     *
     * @return 一个不可变的插件依赖列表
     */
    getDepend(): string[];

    /**
     * 获取提供给其他插件的首选插件列表。
     *
     * @return 一个不可变的插件首选依赖列表
     */
    getSoftDepend(): string[];

    /**
     * 获取应该认为是软依赖的插件列表。
     *
     * @return 可以被视为软依赖的插件列表
     */
    getLoadBefore(): string[];

    /**
     * 获取为插件注册权限的默认值。
     *
     * @return 插件的权限的默认值
     */
    getDefaultPermission(): Enums.PermissionDefault;

    /**
     * 返回此插件将运行时注册的命令列表。
     *
     * @return 此插件将在运行时注册的命令列表
     */
    getCommands(): Command[];

    /**
     * 返回此插件在启用后立即注册的权限列表。
     *
     * @return 此插件在启用后立即注册的权限列表
     */
    getPermissions(): Permission[];
}
