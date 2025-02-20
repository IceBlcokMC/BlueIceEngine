/// <reference path="../../index.d.ts"/>

/**
 * 该类表示一个移动实体（例如：活体实体），如怪物或玩家
 * @hideconstructor
 */
declare class Mob extends Actor {
    toString(): "<Mob>" | string;

    /**
     * 将实体转换为Mob类型
     */
    asMob(): Mob;

    /**
     * 检查是否使用滑翔器或任何其他方式飞行的实体。返回真如果是这样的实体。
     * @return true如果这个实体在滑翔或使用滑翔器等方法飞行。
     */
    isGliding(): boolean;
}
