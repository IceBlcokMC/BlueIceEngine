/// <reference path="../../index.d.ts"/>
/**
 * @hideconstructor
 * 该Level表示，可能包含实体、区块和方块。
 */
declare class Level {
    toString(): "<Level>" | string;

    /**
     * 获取此 Level 的唯一名称
     *
     * @return 此 Level 的名字
     */
    getName(): string;

    /**
     * 获取当前 Level 中所有实体的列表
     *
     * @return 当前在该 Level 中驻留的所有实体的列表
     */
    getActors(): Actor[];

    /**
     * 获取当前服务器上的相对游戏时间。
     *
     * @return 现在的相对时间
     */
    getTime(): number;

    /**
     * 设置服务器上的相对游戏时间。
     *
     * @param time 要设置为相对游戏时间的新时间
     */
    setTime(time: number): void;

    /**
     * 获取此 Level 中的所有维度列表。
     *
     * @return 一个包含所有维度的列表
     */
    getDimensions(): Dimension[];

    /**
     * 根据名称获取给定名称的维度。
     *
     * @param name 要检索的维度的名称
     * @return 包含给定名称的维度，如果不存在则返回 nullptr
     */
    getDimension(name: string): Dimension;
}
