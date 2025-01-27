/// <reference path="../../index.d.ts"/>

declare class ActionForm extends Form {
    constructor();

    toString(): "<ActionForm>" | string;

    getContent(): Message;

    setContent(msg: Message): this;

    addButton(
        text: Message,
        icon?: string,
        callback?: (player: Player) => void
    ): this;

    // getButtons(): any;

    // setButtons(): any;

    // getOnSubmit(): any;

    setOnSubmit(fn: (player: Player, index: number) => void): this;
}
