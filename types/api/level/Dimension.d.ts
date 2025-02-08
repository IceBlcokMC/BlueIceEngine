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
    // getBlocks(location: Location): Block; // TODO: BlockAPI、LocationAPI
}
