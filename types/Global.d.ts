/**
 * 导入一个 Native 类(类似于 import、require)
 * @param NativeClass Native 类名
 */
declare function loadNativeClass<T>(NativeClass: string): T;

/**
 * 当前引擎的ID(仅读)
 * @type {number}
 */
declare const __ENGINE_ID__: number;

declare interface JsPluginBuilder {
  name: string;
  version: string;

  onLoad(fn: () => void): void;
  onEnable(fn: () => void): void;
  onDisable(fn: () => void): void;
}

declare class Engine {
  static registerPlugin(obj: JsPluginBuilder): void;

  static getDeclaration(): string;

  static getSelf(): any; // TODO: endstone::Plugin*
}
