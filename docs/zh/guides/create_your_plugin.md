# 创建你的插件

在开始之前确保您已经安装了以下工具：

- [Node.js](https://nodejs.org/en/)
- [Git(可选)](https://git-scm.com/)
- [VSCode(可选)](https://code.visualstudio.com/)

## 1. 创建插件项目

### 从模板仓库创建

我们提供了一个基于 `TypeScript` 的插件模板仓库，你可以直接克隆这个仓库来创建你的插件项目，也可以点击 `Use this template` 按钮来创建一个新的仓库。

```bash
git clone https://github.com/IceBlcokMC/js_engine_plugin_template.git
cd js_engine_plugin_template
```

### 从零开始创建

你也可以手动从零开始创建一个插件项目，可以参考我们的[模板仓库](https://github.com/IceBlcokMC/js_engine_plugin_template)来了解如何创建一个插件项目，这里不再赘述。

## 2. 安装依赖

```bash
npm install
```

执行此命令后会安装一些必要的依赖，包括 `typescript`、`@types/node` 等。

当 `install` 完成后，会执行 `postinstall` 脚本，它会从 `Js_Engine` 仓库拉取最新的 `types` 文件，并安装到你的项目中。

## 3. 配置插件

使用 `VsCode` 打开你的项目，然后打开 `package.json` 文件，你可以看到以下配置项：

```json
{
  "main": "index.js", // [!code focus]
  "type": "module", // [!code focus]
  "scripts": {
    "build": "tsc",
    "lint": "eslint . --ext .ts",
    "lint:fix": "eslint . --ext .ts --fix",
    "postinstall": "node ./scripts/pull_types.cjs",
    "postbuild": "node scripts/pack.cjs"
  },
  "devDependencies": {
    "@types/node": "^22.10.3",
    "@typescript-eslint/eslint-plugin": "^8.19.0",
    "@typescript-eslint/parser": "^8.19.0",
    "eslint": "^9.17.0",
    "eslint-config-prettier": "^9.1.0",
    "eslint-plugin-n": "^17.15.1",
    "eslint-plugin-prettier": "^5.2.1",
    "eslint-plugin-promise": "^7.2.1",
    "fs-extra": "^11.2.0",
    "prettier": "^3.4.2"
  }
}
```

可以看到，插件项目并没用到 `package.json` 中的全部字段，只使用了一部分，其中 `main` 字段指定入口文件, `Js_Engine` 会通过这个字段来加载你的插件, `type` 字段指定了你的插件是使用 `CommonJS` 还是 `ES Module`，这里我们使用 `ES Module`，所以 `type` 字段为 `module`。

:::warning
这里有一点要注意，如果你的插件使用了 `ES Module`，那么你的 `JSE.registerPlugin` 需要尽可能放在前面，保证最先执行，由于 `ESM` 模块系统异步加载的特性，`Js_Engine` 在加载 `type: module` 的插件时，会预先为插件 post 最多 `15` 次事件循环，所以需要保证 `JSE.registerPlugin` 在这个时间内执行完毕, 否则引擎无法获取到插件的信息。
:::

## 4. 注册命令

在配置好插件项目后，我们打开 `src/index.ts` 文件，注册一个简单的命令：

```typescript
JSE.registerPlugin({
  name: "my_plugin",
  version: "1.0.0",
  onLoad: () => {
    console.log("Plugin loaded!");
  },
  onEnable: () => {
    console.log("Plugin enabled!");
  },
  onDisable: () => {
    console.log("Plugin disabled!");
  },
});
```

插件模板中已经为我们注册了一个 `my_plugin` 插件，我们只需要简单修改一下即可。

```typescript
JSE.registerPlugin({
  name: "helloworld",
  version: "1.0.0",
  // [!code focus:15]
  permissions: {
    hello: {
      default: Enums.PermissionDefault.True,
      description: "Allows the user to say hello",
    },
  },
  commands: {
    hello: {
      description: "Says hello to the user",
      permissions: ["hello"],
      usages: ["/hello"],
    },
  },

  onLoad: () => {
    console.log("Plugin loaded!");
  },
  onEnable: () => {
    console.log("Plugin enabled!");
  },
  onDisable: () => {
    console.log("Plugin disabled!");
  },
});
```

可以看到，我们注册了一个 `helloworld` 插件，并且注册了一个 `hello` 命令，并为 `hello` 命令注册了权限 `hello`。

:::tip
如果你需要为命令添加参数，可以查看 [endstone 文档](https://endstone.dev/latest/tutorials/register-commands/#add-parameters-to-commands)
:::

:::info
`JSE.registerPlugin` 接受一个对象作为参数，该参数描述了插件的元数据，包括插件名称、版本、权限、命令等。
完整的参数列表可查看 [API 文档](https://github.com/IceBlcokMC/Js_Engine/blob/411ecec4566bf2655b91fe498d601a0ba1d4d97f/types/api/jse/JSE.d.ts#L30)
:::

:::warning
`JSE.registerPlugin` 传递的 `object` 只会在插件加载时解析，且这个 `object` 是 `readonly` 的，所以你不能在插件运行时修改这个 `object`。
:::

## 5. 监听命令

在注册完命令后，我们需要监听这个命令，当用户执行这个命令时，我们才能做出相应的处理。

```typescript
JSE.registerPlugin({
  name: "helloworld",
  version: "1.0.0",
  permissions: {
    hello: {
      default: Enums.PermissionDefault.True,
      description: "Allows the user to say hello",
    },
  },
  commands: {
    hello: {
      description: "Says hello to the user",
      permissions: ["hello"],
      usages: ["/hello"],
    },
  },

  onLoad: () => {
    console.log("Plugin loaded!");
  },
  onEnable: () => {
    console.log("Plugin enabled!");
  },
  onDisable: () => {
    console.log("Plugin disabled!");
  },
  // [!code focus:7]
  onCommand(sender, command, args) {
    if (command.getName() === "hello") {
      sender.sendMessage("Hello world!");
    }
    return true;
  },
});
```

这样，当用户执行 `/hello` 命令时，插件就会输出 `Hello world!`。

## 6. 编译并运行插件

在完成插件的编写后，我们需要将插件编译成 JavaScript，然后运行。

```bash
npm run build
```

编译完成后，我们可以在 `dist` 目录下找到编译后的插件文件, 我们把 `dist` 文件夹夹放到 `plugins` 文件夹下，然后运行服务器。

:::tip
当然，你也可以把 `dist` 重命名为你的插件名，这样更好管理。
:::

```bash
endstone
```

在服务器运行后，我们就可以在控制台输入 `/hello` 命令，然后看到输出 `Hello world!`。

```log{7-8,11-12,18-19,23-24}
PS D:\Codes> endstone
[2025-01-23 16:26:48.593 INFO] [EndstoneRuntime] Initialising...
// ...
[2025-01-23 16:26:50.408 INFO] [Server] This server is running Endstone version: 0.5.7.1 (Minecraft: 1.21.50)
[2025-01-23 16:26:51.402 INFO] [JsEngine] Loading js_engine v0.1.0
[2025-01-23 16:26:51.512 INFO] [JsEngine] Load javascript plugin...
[2025-01-23 16:26:51.871 INFO] [Helloworld] Loading helloworld v1.0.0
Plugin loaded!
// ...
[2025-01-23 16:26:54.527 INFO] [JsEngine] Enabling js_engine v0.1.0
[2025-01-23 16:26:54.529 INFO] [Helloworld] Enabling helloworld v1.0.0
Plugin enabled!
? hello
[2025-01-23 16:27:03.221 INFO] [Server] hello:
[2025-01-23 16:27:03.222 INFO] [Server] Says hello to the user
[2025-01-23 16:27:03.223 INFO] [Server] Usage:
[2025-01-23 16:27:03.224 INFO] [Server] - /hello
hello
[2025-01-23 16:27:07.011 INFO] [Server] Hello world!
[2025-01-23 16:27:16.139 INFO] [Server] Server stop requested.
[2025-01-23 16:27:16.158 INFO] [Server] Stopping server...
[2025-01-23 16:27:16.280 INFO] [JsEngine] Disabling js_engine v0.1.0
[2025-01-23 16:27:16.281 INFO] [Helloworld] Disabling helloworld v1.0.0
Plugin disabled!
Quit correctly
```

## 7. 发布你的插件

在完成插件的编写和测试后，你可以将插件发布到插件市场，让其他用户使用。
