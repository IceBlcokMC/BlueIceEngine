declare const __ENGINE_ID__: number;

declare interface JsPluginBuilder {
  name: string;
  version: string;

  onLoad(fn: () => void): void;
  onEnable(fn: () => void): void;
  onDisable(fn: () => void): void;
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
  static registerPlugin(obj: JsPluginBuilder): void;

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
