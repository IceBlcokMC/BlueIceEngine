declare module "JSEAPI" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class JSEAPI {
        static registerPlugin(p0: TestBuilder) :boolean;
        static isWindows() :boolean;
        static isLinux() :boolean;
    }

}
declare module "TestBuilder" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class TestBuilder {
        constructor();
        name: string;
        version: string;
        description: string;
        onLoad: () => void;
        onEnable: () => void;
        onDisable: () => void;
    }

}
