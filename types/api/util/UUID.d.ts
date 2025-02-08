/// <reference path="../../index.d.ts"/>

/**
 * 实现 universally unique identifier（UUID）。
 *
 * 引用自 https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp
 */
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
