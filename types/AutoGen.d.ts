declare module ""PluginDescriptionBuilder"" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class "PluginDescriptionBuilder" {
        description: string;
        load: "PluginLoadOrder";
        authors: string[];
        contributors: string[];
        website: string;
        prefix: string;
        provides: string[];
        depend: string[];
        soft_depend: string[];
        load_before: string[];
    }

}
declare module "JSEAPI" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class JSEAPI {
        static registerPlugin(p0: NativeBuilder) :boolean;
        static isWindows() :boolean;
        static isLinux() :boolean;
    }

}
declare module "NativeBuilder" {
    import {$Ref, $Nullable, cstring} from "puerts"

    class NativeBuilder extends "PluginDescriptionBuilder" {
        constructor(p0: string, p1: string);
        name: string;
        version: string;
    }

}
