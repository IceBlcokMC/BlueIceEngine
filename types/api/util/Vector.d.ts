/// <reference path="../../index.d.ts"/>

/**
 * 表示一个三维向量。
 * Vector<float>
 */
declare class Vector {
    constructor(x: number, y: number, z: number);

    x: number; // float
    y: number; // float
    z: number; // float

    toString(): string;

    /**
     * 获取X分量。
     *
     * @return X分量。
     */
    getX(): number; // float

    /**
     * 获取Y分量。
     *
     * @return Y分量。
     */
    getY(): number; // float

    /**
     * 获取Z分量。
     *
     * @return Z分量。
     */
    getZ(): number; // float

    /**
     * 设置X分量。
     *
     * @param x 新的X分量。
     * @return 这个向量。
     */
    setX(x: number): number; // float

    /**
     * 设置Y分量。
     *
     * @param y 新的Y分量。
     * @return 这个向量。
     */
    setY(y: number): number; // float

    /**
     * 设置Z分量。
     *
     * @param z 新的Z分量。
     * @return 这个向量。
     */
    setZ(z: number): number; // float

    /**
     * 获取向量的模，定义为sqrt(x^2 + y^2 + z^2)。
     *
     * @return 模长
     */
    length(): number; // float

    /**
     * 获取向量的平方模。
     *
     * @return 模长的平方
     */
    lengthSquared(): number; // float

    /**
     * 获取从当前向量到另一个向量的距离。
     *
     * @param other 其他向量
     * @return 距离
     */
    distance(other: Vector): number; // float

    /**
     * 获取从当前向量到另一个向量的平方距离。
     *
     * @param other 其他向量
     * @return 距离
     */
    distanceSquared(other: Vector): number; // float
}
