/// <reference path="../index.d.ts"/>

declare interface OfflinePlayer {
    /**
     * Returns the name of this player
     * 返回玩家的名字
     *
     * @returns Player name or null if we have not seen a name for this player yet
     * @returns 玩家的名字，如果尚未看到该玩家的名字，则返回null
     */
    getName(): string;

    /**
     * Returns the UUID of this player
     * 返回玩家的UUID
     */
    getUniqueId(): UUID;
}
