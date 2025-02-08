/// <reference path="../../index.d.ts"/>

/**
 * 该位置的三维维度表示，包含在维度中的一级。
 */
declare class Position extends Vector {
    constructor(dim: Dimension, x: number, y: number, z: number);

    toString(): "<Position>" | string;

    /**
     * 获取这个位置所在的维度
     *
     * @return 包含这个位置的维度对象，或如果未设置，则返回null。
     */
    getDimension(): Dimension;

    /**
     * 设置这个位置所在的维度
     *
     * @param dimension 新的维度对象，该位置将位于其中
     */
    setDimension(dim: Dimension): void;

    /**
     * 获取X组件的下确界值。这表示这个位置所包含的块ID为多少。
     *
     * @return 块 X
     */
    getBlockX(): number;

    /**
     * 获取Y组件的下确界值。这表示这个位置所包含的块ID为多少。
     *
     * @return 块 Y
     */
    getBlockY(): number;

    /**
     * 获取Z组件的下确界值。这表示这个位置所包含的块ID为多少。
     *
     * @return 块 Z
     */
    getBlockZ(): number;
}
