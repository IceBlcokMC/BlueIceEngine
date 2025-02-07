/// <reference path="../index.d.ts"/>

declare namespace Enums {
    /**
     * 表示游戏类型的接口。
     */
    enum GameMode {
        /**
         * 生存模式是“正常”游戏类型，没有特殊功能。
         */
        Survival = 0,
        /**
         * 创造模式可以飞行、不可攻击，可以随意获取物品。
         */
        Creative = 1,
        /**
         * 冒险模式无法使用正确的工具损坏方块。
         */
        Adventure = 2,
        /**
         * 观察者模式无法在任何方式与世界互动，并且对普通玩家是不可见的。这授予玩家通过世界无碰撞的能力。
         */
        Spectator = 3,
    }
}
