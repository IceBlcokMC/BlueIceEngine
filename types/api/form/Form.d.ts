/// <reference path="../../index.d.ts"/>

/**
 * @hideconstructor
 * 该类表示一个通用表单。
 * Form类提供了一个不同的类型的表单的基抽象，支持三种类型：Action、Message和Modal。每个表单都有一个标题，可以是一个字符串或一个可翻译对象。
 */
declare class Form {
    /**
     * 获取表单的标题。
     *
     * @return 表单的标题。
     */
    getTitle(): string;

    /**
     * 设置表单的标题。
     *
     * @param title 要设置为表单的标题，可以是字符串或一个可翻译对象。
     * @return 当前表单的引用。
     */
    setTitle(title: string): this;

    /**
     * 设置表单关闭时的回调函数。
     *
     * @param on_close 回调函数要设置为。
     * @return 当前表单的引用。
     */
    setOnClose(on_close: (player: Player | undefined) => void): this;

    /**
     * 获取表单关闭时的回调函数。
     *
     * @return 表单关闭时的回调函数。
     */
    // getOnClose(): fn
}
