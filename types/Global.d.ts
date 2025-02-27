declare const __ENGINE_ID__: number;

declare enum PluginLoadOrder {
  /**
   * 指示插件将在启动时加载
   */
  Startup = 0,
  /**
   * 表示该插件将与第一个/默认世界创建后加载
   */
  PostWorld = 1,
}

declare enum PermissionDefault {
  /** 所有人都可以执行 */
  True = 0,
  /** 玩家不可执行 */
  False = 1,
  /** 仅管理员可执行 */
  Operator = 2,
  /** 仅玩家可执行 */
  NotOperator = 3,
}

declare type PermissionBuilder = {
  [K: string]: {
    /** 权限描述 */
    description: string;
    /** 默认权限 */
    default: PermissionDefault;
  };
};

// https://endstone.readthedocs.io/en/latest/tutorials/register-commands/#built-in-types
declare type CommandBuilder<T extends PermissionBuilder> = {
  [key: string]: {
    /** 命令描述 */
    description: string;
    /** 命令用法 */
    usages: string[];
    /** 命令权限 */
    permissions: Array<keyof T>;
  };
};

declare interface JsPluginBuilder<T extends PermissionBuilder> {
  name: string;
  version: string;
  description?: string;
  load?: PluginLoadOrder;
  authors?: string[];
  contributors?: string[];
  website?: string;
  prefix?: string;
  provides?: string[];
  depends?: string[];
  soft_depend?: string[];
  load_before?: string[];
  default_permission?: PermissionDefault;
  permissions?: T;
  commands?: CommandBuilder<T>;

  onLoad(fn: () => void): void;
  onEnable(fn: () => void): void;
  onDisable(fn: () => void): void;
  onCommand(fn: () => void): boolean; // TODO: argument
}

declare class Engine {
  /**
   * 导入一个 Native Bind 类
   * @param class_ 类名
   */
  static loadNativeClass<T extends NativeClasses>(class_: T): NativeTypeMap[T];

  /**
   * 注册一个 JS 插件
   * @param obj 注册对象
   */
  static registerPlugin<T extends PermissionBuilder>(
    obj: JsPluginBuilder<T>
  ): void;

  /**
   * 生成所有 Native Bind 类的声明文件
   */
  static getDeclaration(): string;

  /**
   * 获取注册的插件实例
   * @warning 调用此函数前需要先注册插件实例，且必须在 onLoad 触发后调用，否则抛出异常
   */
  static getSelf(): endstone.Plugin; // endstone::Plugin*
}
