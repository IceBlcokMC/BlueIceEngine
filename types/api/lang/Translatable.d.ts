/// <reference path="../../index.d.ts"/>

/**
 * 该对象具有文本表示，可以被Minecraft客户端翻译。
 */
declare class Translatable {
    constructor(text: string, params: string[]);

    toString(): "<Translatable>" | string;

    /**
     * @brief 获取要翻译的文本。
     *
     * @return 要翻译的文本
     */
    getText(): string;

    /**
     * @brief 获取翻译参数。
     *
     * @return 翻译参数
     */
    getParameters(): string[];

    /**
     * 该方法检查要翻译的消息是否为空。
     *
     * @return 如果要翻译的消息为空，则为true，否则为false
     */
    empty(): boolean;
}
