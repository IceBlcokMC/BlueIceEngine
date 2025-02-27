/**
 * 加载一个 NativeBinding 的原生类
 * @param NativeClass NativeBinding 时绑定的类名
 */
declare function loadNativeClass<T>(NativeClass: string): T;

/**
 * 获取所有 NativeBinding 的类型声明
 * @returns {string}
 */
declare function __declaration__(): string;

/**
 * 当前引擎的ID
 * @warning 此参数禁止修改, 如果修改引擎会抛出异常
 * @type {number}
 */
declare const __ENGINE_ID__: number;

type BuilderFunc = () => void;

declare interface JsPluginBuilder {
  name: string;
  version: string;

  onLoad(fn: BuilderFunc): void;
  onEnable(fn: BuilderFunc): void;
  onDisable(fn: BuilderFunc): void;
}

declare class Engine {
  static registerPlugin(obj: JsPluginBuilder): void;

  static getSelf(): any; // TODO: endstone::Plugin*
}
