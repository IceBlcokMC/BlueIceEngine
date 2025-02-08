/// <reference path="../../index.d.ts"/>

/**
 * 该类表示一个包含按钮的表单，允许玩家执行操作。
 */
declare class ActionForm extends Form {
    constructor();

    toString(): "<ActionForm>" | string;
    /**
     * 获取表单的内容。
     *
     * @return 表单的内容。
     */
    getContent(): Message;

    /**
     * 设置表单的内容。
     *
     * @param msg 要设置为表单内容的文字。
     * @return 当前表单的引用。
     */
    setContent(msg: Message): this;

    /**
     * 添加按钮到表单中。
     *
     * @param text 按钮的文本
     * @param icon 图标图片的路径或URL
     * @param callback 按钮点击时的回调函数
     * @return 当前表单的引用。
     */
    addButton(
        text: Message,
        icon?: string,
        callback?: (player: Player) => void
    ): this;

    /**
     * 获取动作表单中的按钮。
     *
     * @return 动作表单中的按钮列表。
     */
    // getButtons(): any;

    /**
     * 设置动作表单中的按钮。
     *
     * @param buttons 要设置的按钮列表。
     * @return 当前表单的引用。
     */
    // setButtons(): any;

    /**
     * 获取表单提交时的回调函数。
     *
     * @return 表单提交时的回调函数。
     */
    // getOnSubmit(): any;

    /**
     * 设置表单提交时的回调函数。
     *
     * @param fn 回调函数。
     * @return 当前表单的引用。
     */
    setOnSubmit(fn: (player: Player, index: number) => void): this;
}
