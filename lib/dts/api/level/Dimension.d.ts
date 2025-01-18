/// <reference path="../../index.d.ts"/>

/**
 * @hideconstructor
 */
declare class Dimension {
    toString(): "<Dimension>" | string;

    getName(): string;

    getType(): Enums.DimensionType;

    getLevel(): Level;

    // getBlockAt(x: number, y: number, z: number): Block; // TODO: BlockAPI

    // getBlocks(location: Location): Block; // TODO: BlockAPI、LocationAPI
}
