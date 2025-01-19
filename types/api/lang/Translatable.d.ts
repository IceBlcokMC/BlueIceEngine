/// <reference path="../../index.d.ts"/>


declare class Translatable {
    constructor(text: string, params: string[]);

    toString(): "<Translatable>" | string;

    getText(): string;

    getParameters(): string[];

    empty(): boolean;
}
