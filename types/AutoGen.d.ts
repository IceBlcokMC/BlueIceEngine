declare namespace endstone {
    enum PermissionDefault {
        False = 1,
        NotOperator = 3,
        Operator = 2,
        True = 0,
    }

    enum PluginLoadOrder {
        PostWorld = 1,
        Startup = 0,
    }

}

declare type NativeTypeMap = {
    "endstone::CommandExecutor": endstone.CommandExecutor,
    "endstone::Plugin": endstone.Plugin,
}

declare type NativeClasses = keyof NativeTypeMap;

declare namespace endstone {
    /** @hideconstructor */
    class CommandExecutor {
    }

    /** @hideconstructor */
    class Plugin extends CommandExecutor {
        onLoad(): void;
        onEnable(): void;
        onDisable(): void;
        getName(): string;
    }

}
