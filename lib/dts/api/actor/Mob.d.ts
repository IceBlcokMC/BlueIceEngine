/// <reference path="../../index.d.ts"/>

/**
 * 实体API
 * @hideconstructor
 */
declare class Mob extends Actor {
    toString(): "<Mob>" | string;

    /** 实体是否在滑翔 */
    isGliding(): boolean;
}
