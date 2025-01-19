/// <reference path="../../index.d.ts"/>

declare class UUID {
    constructor(uuid: string | UUID);

	toString(): "<UUID>" | string;

    forEach(callback: (u8: number) => void): void;

    size(): number; // size_t

    isNil(): boolean;

    version(): number;

    swap(): void;

    str(): string;
}
