/// <reference path="../index.d.ts"/>

declare class Server {
    toString(): "<Server>" | string;

    getName(): string;

    getVersion(): string;

    getMinecraftVersion(): string;

    // getLogger(): any;

    // getLanguage(): any;

    // getPluginManager(): any;

    // getPluginCommand(): any;

    // getCommandSender(): any;

    dispatchCommand(sender: CommandSender, command_line: string): any;

    // getScheduler(): any;

    // getLevel(): any;

    getOnlinePlayers(): Player[];

    getMaxPlayers(): number;

    setMaxPlayers(max_players: number): void;

    getPlayer(name_or_uuid: string | UUID): Player;

    getOnlineMode(): boolean;

    shutdown(): void;

    reload(): void;

    reloadData(): void;

    broadcast(message: string | Translatable, permission: string): void; // TODO: replace message type to pair<Message, Translatable>

    broadcastMessage(message: string): void;

    isPrimaryThread(): boolean;

    // getScoreboard(): any;

    // createScoreboard(): any;

    getCurrentMillisecondsPerTick(): number;

    getAverageMillisecondsPerTick(): number;

    getCurrentTicksPerSecond(): number;

    getAverageTicksPerSecond(): number;

    getCurrentTickUsage(): number;

    getAverageTickUsage(): number;

    getStartTime(): number;

    // createBossBar(): any;

    // createBlockData(): any;

    // getBanList(): any;

    // getIpBanList(): any;

    static get BroadcastChannelAdmin(): string;
    static get BroadcastChannelUser(): string;
}
