declare enum PluginLoadOrder {
    Startup,
    PostWorld
}

declare class PluginLoader {
// TODO
}

declare class PluginCommand {
// TODO
}

declare class PluginDescription {
    constructor();// TODO

    readonly name: string;

    readonly version: string;

    readonly fullName: string;

    readonly apiVersion: string;

    readonly description: string;

    readonly load: PluginLoadOrder;

    readonly authors: string[];

    readonly contributors: string[];

    readonly website: string;

    readonly prefix: string;

    readonly provides: string[];

    readonly depend: string[];

    readonly softDepend: string[];

    readonly loadBefore: string[];

    readonly defaultPermission; // TODO

    readonly commands;// TODO

    readonly permissions;// TODO
}

declare class Plugin {
    constructor();

    onLoad(): void;

    onEnable(): void;

    onDisable(): void;

    getDescription(): PluginDescription;

    onCommand(): boolean; // TODO

    readonly logger // TODO

    readonly pluginLoader // TODO

    readonly server // TODO

    readonly isEnabled: boolean

    readonly name: string

    getCommand() // TODO

    readonly dataFolder: string;
}
