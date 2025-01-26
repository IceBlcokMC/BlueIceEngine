/// <reference path="../../index.d.ts"/>

/**
 * @hideconstructor
 */
declare class Form {
    getTitle(): string;

    setTitle(title: string): this;

    setOnClose(callback: (player: Player | undefined) => void): this;

    // getOnClose(): fn
}
