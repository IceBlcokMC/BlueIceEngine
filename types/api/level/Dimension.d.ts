/// <reference path="../../index.d.ts"/>

/**
 * @hideconstructor
 * 该维度表示级别中的一级。
 */
declare class Dimension {
    toString(): "<Dimension>" | string;

    /**
     * 获取此维度的名称
     *
     * @return 此维度的名称
     */
    getName(): string;

    /**
     * 获取此维度的类型
     *
     * @return 此维度的类型
     */
    getType(): Enums.DimensionType;

    /**
     * 获取包含该维度的级别。
     *
     * @return 包含该维度的级别。
     */
    getLevel(): Level;

    /**
     * 根据给定坐标获取块。
     *
     * @param x 块的 X 坐标
     * @param y 块的 Y 坐标
     * @param z 块的 Z 坐标
     * @return 包含给定坐标的块
     */
    // getBlockAt(x: number, y: number, z: number): Block; // TODO: BlockAPI

    /**
     * 根据给定位置获取块。
     *
     * @param location 块的位置
     * @return 包含给定坐标的块
     */
    // getBlocks(location: Location): Block; // TODO: BlockAPI

    /**
     * 获取给定坐标的最高块。
     * @param x 方块的 X 坐标
     * @param z 方块的 Z 坐标
     */
    getHighestBlockYAt(x: number, z: number): number;

    /**
     * 获取给定位置的最高块。
     * @param location 方块的位置
     */
    // getHighestBlockAt(location: Location): Block;

    /**
     * 获取给定坐标的最高块。
     * @param x 方块的 X 坐标
     * @param z 方块的 Z 坐标
     * @return 包含给定坐标的最高块的块
     */
    // getHighestBlockAt(x: number, z: number): Block;

    /**
     * 获取此维度中已加载的块。
     * @return 此维度中已加载的块
     */
    // getLoadedChunks(): Chunk[];
}
