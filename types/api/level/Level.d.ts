/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 */
declare class Level {
    toString(): "<Level>" | string;

    getName(): string;

    getActors(): Actor[];

    getTime(): number;

    setTime(time: number): void;

    getDimensions(): Dimension[];

    getDimension(name: string): Dimension;
}
