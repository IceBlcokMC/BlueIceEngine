/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 */
declare class Location extends Position {
    toString(): "<Location>" | string;

    getPitch(): number;

    setPitch(pitch: number): void;

    getYaw(): number;

    setYaw(): number;
}
