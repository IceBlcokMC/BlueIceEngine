declare module "endstone::CommandExecutor" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class endstone::CommandExecutor {
    }

}
declare module "endstone::Plugin" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class endstone::Plugin extends endstone::CommandExecutor {
        onLoad() :void;
        onEnable() :void;
        onDisable() :void;
        getName() :string;
    }

}
