/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 */
declare class Position extends Vector {
    toString(): "<Position>" | string;

    getDimension(): Dimension;

    setDimension(dim: Dimension): void;

    getBlockX(): number;

    getBlockY(): number;

    getBlockZ(): number;
}
