/// <reference path="../../index.d.ts"/>

declare class Translatable {
    toString(): "<Translatable>" | string;

    getText(): string;

    getParameters(): string[];

    empty(): boolean;
}
