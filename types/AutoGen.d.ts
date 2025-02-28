//----------------------------------------------------------
// Version:  0.1.0
//----------------------------------------------------------

//----------------------------------------------------------
// NativeTypeMap
//----------------------------------------------------------
declare type NativeTypeMap = {
    "endstone::CommandExecutor": endstone.CommandExecutor,
    "endstone::Plugin": endstone.Plugin,
}
declare type NativeClasses = keyof NativeTypeMap;


//----------------------------------------------------------
// Native Enums
//----------------------------------------------------------
declare enum PermissionDefault {
    False = 1,
    NotOperator = 3,
    Operator = 2,
    True = 0,
}

declare enum PluginLoadOrder {
    PostWorld = 1,
    Startup = 0,
}



//----------------------------------------------------------
// Native Classes
//----------------------------------------------------------
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
