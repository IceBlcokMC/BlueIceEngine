// /// <reference path="../js_engine/dts/index.d.ts"/>

// const { JsonFileConfig } = require("../js_engine/shortcutApi/JsonFileConfig.js");

const assert = (condition) => {
  if (!condition) {
    throw new Error(`assert failed!`);
  }
};

const Print_Test_Title = (title) =>
  JSE.debug(`===================> Test ${title} <===================`);

/* Test Code */
function TEST_Enums() {
  Print_Test_Title("Enums");

  assert(Enums.PluginLoadOrder.Startup === 0);
  assert(Enums.PluginLoadOrder.PostWorld === 1);
}

function TEST_Plugin() {
  Print_Test_Title("Plugin");

  const plugin = JSE.getSelf();
  JSE.debug(`isEnabled: `, plugin.isEnabled());
  JSE.debug(`getName: `, plugin.getName());
  JSE.debug(`getDataFolder: `, plugin.getDataFolder());
}

function TEST_Logger() {
  Print_Test_Title("Logger");

  const logger = JSE.getSelf().getLogger();
  logger.setLevel(0);
  logger.log(Enums.LoggerLevel.Info, "logger log");
  logger.debug("logger debug");
  logger.info("logger info");
  logger.warning("logger warning");
  logger.error("logger error");
  logger.trace("logger trace");
  logger.critical("logger critical");
  logger.info(`enable: ${logger.isEnabledFor(5)}, name: ${logger.getName()}`);
  logger.info(
    new Array(3)
      .fill()
      .map((_, i) => `Test Line: ${i}`)
      .join("\n")
  );
}

function TEST_PluginDescription() {
  Print_Test_Title("PluginDescription");

  const description = JSE.getSelf().getDescription();
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
  JSE.debug("getDefaultPermission: ", description.getDefaultPermission());
}

function TEST_toString() {
  Print_Test_Title("Custom Class toString");

  const plugin = JSE.getSelf();
  const logger = plugin.getLogger();
  const description = plugin.getDescription();

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
}

function TEST_MessageForm() {
  Print_Test_Title("MessageForm");

  const fm = new MessageForm();
  fm.setTitle("Test")
    .setContent("AAA 建材批发")
    .setButton1("10086")
    .setButton2("close")
    .setOnSubmit((pl, num) => {
      JSE.debug("setOnSubmit, pl: ", pl);
    })
    .setOnClose((pl) => {
      JSE.debug("setOnClose, pl: ", pl);
    });

  JSE.getSelf()
    ?.getServer()
    ?.getOnlinePlayers()
    ?.forEach((pl) => pl.sendForm(fm));
}

function TEST_ActionForm() {
  Print_Test_Title("ActionForm");

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

  JSE.getSelf()
    ?.getServer()
    ?.getOnlinePlayers()
    ?.forEach((pl) => pl.sendForm(fm));
}

function TEST_UUID() {
  Print_Test_Title("UUID");
  // engsr6982: 82c5615f-92be-32b1-83c8-b937284d9de6

  const player = JSE.getSelf().getServer().getPlayer("engsr6982");
  if (!player) {
    JSE.debug("Player not found");
    return;
  }

  const UUIDSTR = `82c5615f-92be-32b1-83c8-b937284d9de6`;

  const playerUUID = player.getUniqueId();
  const playerUUIDStr = playerUUID.str();
  assert(playerUUIDStr === UUIDSTR); // 确保UUID相同

  const newUUID = new UUID(UUIDSTR);
  assert(newUUID === playerUUID); // 测试字符串构造

  const newUUIDStr = newUUID.str();
  assert(newUUIDStr === UUIDSTR); // 测试toString
  assert(newUUIDStr === playerUUIDStr); // 测试toString
}

function TEST_Native_StackTrace() {
  Print_Test_Title("Native StackTrace");
  throw new Error("Test Native StackTrace");
}

/**
 * @param {CommandSender} sender
 * @param {Command} cmd
 * @param {string[]} args
 * @returns
 */
function onCommand(sender, cmd, args) {
  if (cmd.getName() !== "test") {
    return true;
  }

  switch (args[0]) {
    case "stacktrace":
      TEST_Native_StackTrace();
      break;

    case "uuid":
      TEST_UUID();
      break;

    case "enums":
      TEST_Enums();
      break;

    case "plugin":
      TEST_Plugin();
      break;

    case "logger":
      TEST_Logger();
      break;

    case "plugindesc":
      TEST_PluginDescription();
      break;

    case "tostr":
      TEST_toString();
      break;

    case "msgform":
      TEST_MessageForm();
      break;

    case "actionform":
      TEST_ActionForm();
      break;

    default:
      sender.sendErrorMessage("Invalid module");
  }
  return true;
}

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
      usages: ["/test <module: string>"],
      permissions: ["test.a1"],
    },
  },

  onLoad: () => {
    JSE.debug("onLoad called");
  },

  onEnable: () => {
    JSE.debug("onEnable called");
  },

  onDisable: () => {
    JSE.debug("onDisable called");
  },

  onCommand: onCommand,
});
