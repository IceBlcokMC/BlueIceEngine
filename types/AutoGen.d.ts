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
        onLoad() :void;
        onEnable() :void;
        onDisable() :void;
        getName() :string;
    }

}
