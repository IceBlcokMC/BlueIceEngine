/// <reference path="../../index.d.ts"/>

declare class MessageForm extends Form {
    constructor();

    toString(): "<MessageForm>" | string;

    getContent(): Message;

    setContent(msg: Message): this;

    getButton1(): Message;

    setButton1(msg: Message): this;

    getButton2(): Message;

    setButton2(msg: Message): this;

    setOnSubmit(fn: (player: Player, index: number) => void): this;

    // getOnSubmit(): (player: Player, num: number) => void;
}
