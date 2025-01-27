// /// <reference path="../js_engine/dts/index.d.ts"/>

// const { JsonFileConfig } = require("../js_engine/shortcutApi/JsonFileConfig.js");

JSE.registerPlugin({
    name: "test_js_plugin",
    version: "114514",
    description: "测试",
    load: Enums.PluginLoadOrder.PostWorld,
    authors: ["engsr6982"],
    contributors: ["engsr6982", "zimuya4153"],
    website: "https://github.com/engsr6982",
    prefix: "JSPluginTest",
    permissions: {
        "test.a1": {
            description: "test permission",
            default: Enums.PermissionDefault.True,
        },
    },
    commands: {
        test: {
            description: "test command",
            usages: ["/test"],
            permissions: ["test.a1"],
        },
    },

    onLoad: () => {
        JSE.debug("onLoad called");
    },

    onEnable: () => {
        JSE.debug("onEnable called");
        const plugin = JSE.getSelf(),
            logger = plugin.getLogger(),
            description = plugin.getDescription();

        JSE.debug("Enums: ", Enums);
        JSE.debug(
            "Enums.PluginLoadOrder.PostWorld：",
            Enums.PluginLoadOrder.PostWorld
        );

        (() => {
            JSE.debug("==== Test Plugin ====");
            JSE.debug(`isEnabled: `, plugin.isEnabled());
            JSE.debug(`getName: `, plugin.getName());
            JSE.debug(`getDataFolder: `, plugin.getDataFolder());
        })();

        (() => {
            JSE.debug("==== Test Logger ====");
            logger.setLevel(0);
            logger.log(Enums.LoggerLevel.Info, "logger log");
            logger.debug("logger debug");
            logger.info("logger info");
            logger.warning("logger warning");
            logger.error("logger error");
            logger.trace("logger trace");
            logger.critical("logger critical");
            logger.info(
                `enable: ${logger.isEnabledFor(5)}, name: ${logger.getName()}`
            );
            logger.info(
                new Array(3)
                    .fill()
                    .map((_, i) => `Test Line: ${i}`)
                    .join("\n")
            );
        })();

        (() => {
            JSE.debug("==== Test PluginDescription ====");
            JSE.debug("getName: ", description.getName());
            JSE.debug("getVersion: ", description.getVersion());
            JSE.debug("getFullName: ", description.getFullName());
            JSE.debug("getAPIVersion: ", description.getAPIVersion());
            JSE.debug("getDescription: ", description.getDescription());
            JSE.debug("getLoad: ", description.getLoad());
            JSE.debug("getAuthors: ", description.getAuthors());
            JSE.debug("getContributors: ", description.getContributors());
            JSE.debug("getWebsite: ", description.getWebsite());
            JSE.debug("getPrefix: ", description.getPrefix());
            JSE.debug("getProvides: ", description.getProvides());
            JSE.debug("getDepend: ", description.getDepend());
            JSE.debug("getSoftDepend: ", description.getSoftDepend());
            JSE.debug("getLoadBefore: ", description.getLoadBefore());
            JSE.debug(
                "getDefaultPermission: ",
                description.getDefaultPermission()
            );
        })();

        (() => {
            JSE.debug("==== UUID Test ====");
        })();

        (() => {
            JSE.debug("==== Custom Class toString ====");
            JSE.debug(`Logger: `, logger);
            JSE.debug(`Plugin: `, plugin);
            JSE.debug(`PluginDescription: `, description);
            JSE.debug(`Test Class1: `, new (class Test1 {})());
            JSE.debug(
                `Test Class2: `,
                new (class Test2 {
                    toString() {
                        return "<Test Class2>";
                    }
                })()
            );
        })();
    },

    onDisable: () => {
        JSE.debug("onDisable called");
    },

    onCommand: (sender, cmd, args) => {
        JSE.debug("onCommand called");
        JSE.debug("sender: ", sender);
        JSE.debug("cmd: ", cmd);
        sender.sendMessage("Hello: ", sender.getName());
        sender.sendMessage("cmd: ", cmd.getName());
        console.warn("args: ", args);
        JSE.debug("parent class isOp: ", sender.isOp);
        JSE.debug("parent class isOp call: ", sender.isOp());

        JSE.debug("MessageForm: ", typeof MessageForm);

        if (cmd.getName() == "test") {
            // const fm = new MessageForm();
            // fm.setTitle("Test")
            //     .setContent("AAA 建材批发")
            //     .setButton1("10086")
            //     .setButton2("close")
            //     .setOnSubmit((pl, num) => {
            //         JSE.debug("setOnSubmit, pl: ", pl);
            //     })
            //     .setOnClose((pl) => {
            //         JSE.debug("setOnClose, pl: ", pl);
            //     });
            const fm = new ActionForm();
            fm.setTitle("Test ActionForm")
                .setContent("测试")
                .setOnClose((pl) => {
                    JSE.debug("ActionForm onClose, pl: ", pl);
                })
                .setOnSubmit((pl, data) => {
                    JSE.debug("ActionForm onSubmit, pl: ", pl);
                    JSE.debug("ActionForm onSubmit, data: ", data);
                });

            for (let index = 0; index < 10; index++) {
                fm.addButton(`Button #${index}`, "", (pl) => {
                    pl.sendMessage(`clicked #${index}`);
                });
            }

            const player = sender.asPlayer();
            if (player) {
                player.sendForm(fm);
                JSE.debug("sendForm");
            } else {
                JSE.debug("player is null");
            }
        }

        return true;
    },
});
