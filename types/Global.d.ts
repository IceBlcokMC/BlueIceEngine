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
 * @type {number}
 */
declare const __ENGINE_ID__: number;
