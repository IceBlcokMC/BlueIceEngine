/// <reference path="../../index.d.ts"/>

declare class Location extends Position {
    constructor(
        dim: Dimension,
        x: number,
        y: number,
        z: number,
        pitch: number,
        yaw: number
    );

    toString(): "<Location>" | string;

    getPitch(): number;

    setPitch(pitch: number): void;

    getYaw(): number;

    setYaw(): number;
}
