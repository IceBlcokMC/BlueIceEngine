/// <reference path="../../index.d.ts"/>

/**
 * 该类表示一个包含两个按钮的表单。
 */
declare class MessageForm extends Form {
    constructor();

    toString(): "<MessageForm>" | string;

    /**
     * 获取表单的内容。
     *
     * @return 表单的内容。
     */
    getContent(): Message;

    /**
     * 设置表单的内容。
     *
     * @param text 要设置为表单内容的文字。
     * @return 当前表单的引用。
     */
    setContent(text: Message): this;

    /**
     * 获取按钮1的文字。
     *
     * @return 按钮1的文字。
     */
    getButton1(): Message;

    /**
     * 设置按钮1的文字。
     *
     * @param text 要设置为按钮1文字的文字。
     * @return 当前表单的引用。
     */
    setButton1(text: Message): this;

    /**
     * 获取按钮2的文字。
     *
     * @return 按钮2的文字。
     */
    getButton2(): Message;

    /**
     * 设置按钮2的文字。
     *
     * @param text 要设置为按钮2文字的文字。
     * @return 当前表单的引用。
     */
    setButton2(text: Message): this;

    /**
     * 设置表单提交时的回调函数。
     *
     * @param on_submit 回调函数要设置为。
     * @return 当前表单的引用。
     */
    setOnSubmit(on_submit: (player: Player, index: number) => void): this;

    /**
     * 获取表单提交时的回调函数。
     *
     * @return 表单提交时的回调函数。
     */
    // getOnSubmit(): (player: Player, num: number) => void;
}
