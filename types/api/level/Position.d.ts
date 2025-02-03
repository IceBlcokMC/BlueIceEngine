/// <reference path="../../index.d.ts"/>

declare class Position extends Vector {
    constructor(dim: Dimension, x: number, y: number, z: number);

    toString(): "<Position>" | string;

    getDimension(): Dimension;

    setDimension(dim: Dimension): void;

    getBlockX(): number;

    getBlockY(): number;

    getBlockZ(): number;
}
