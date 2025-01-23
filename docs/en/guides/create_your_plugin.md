# Create Your Plugin

Before getting started, make sure you have the following tools installed:

- [Node.js](https://nodejs.org/en/)
- [Git (optional)](https://git-scm.com/)
- [VSCode (optional)](https://code.visualstudio.com/)

## 1. Create Plugin Project

### Create from Template Repository

We provide a plugin template repository based on `TypeScript`. You can either clone this repository directly to create your plugin project, or click the `Use this template` button to create a new repository.

```bash
git clone https://github.com/IceBlcokMC/js_engine_plugin_template.git
cd js_engine_plugin_template
```

### Create from Scratch

You can also manually create a plugin project from scratch. You can refer to our [template repository](https://github.com/IceBlcokMC/js_engine_plugin_template) to learn how to create a plugin project. We won't go into details here.

## 2. Install Dependencies

```bash
npm install
```

This command will install necessary dependencies, including `typescript`, `@types/node`, etc.

After the `install` is complete, the `postinstall` script will run, which pulls the latest `types` files from the `Js_Engine` repository and installs them in your project.

## 3. Configure Plugin

Open your project with `VSCode`, then open the `package.json` file. You'll see the following configuration items:

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

As you can see, the plugin project doesn't use all fields in `package.json`, only a portion. The `main` field specifies the entry file, which `Js_Engine` will use to load your plugin. The `type` field specifies whether your plugin uses `CommonJS` or `ES Module`. Here we use `ES Module`, so the `type` field is set to `module`.

:::warning
Note that if your plugin uses `ES Module`, your `JSE.registerPlugin` needs to be placed as early as possible to ensure it executes first. Due to the asynchronous loading nature of the `ESM` module system, `Js_Engine` will pre-post up to `15` event loops when loading plugins with `type: module`. Therefore, you need to ensure that `JSE.registerPlugin` completes within this time, otherwise the engine cannot obtain the plugin's information.
:::

## 4. Register Commands

After configuring the plugin project, open the `src/index.ts` file and register a simple command:

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

The plugin template has already registered a `my_plugin` plugin for us. We just need to make some simple modifications.

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

As you can see, we registered a `helloworld` plugin and registered a `hello` command with the `hello` permission.

:::tip
If you need to add parameters to commands, you can check the [endstone documentation](https://endstone.dev/latest/tutorials/register-commands/#add-parameters-to-commands)
:::

:::info
`JSE.registerPlugin` accepts an object as a parameter that describes the plugin's metadata, including plugin name, version, permissions, commands, etc.
For a complete list of parameters, see the [API documentation](https://github.com/IceBlcokMC/Js_Engine/blob/411ecec4566bf2655b91fe498d601a0ba1d4d97f/types/api/jse/JSE.d.ts#L30)
:::

:::warning
The `object` passed to `JSE.registerPlugin` is only parsed when the plugin loads, and this `object` is `readonly`, so you cannot modify this `object` during plugin runtime.
:::

## 5. Listen for Commands

After registering the command, we need to listen for this command so we can handle it when users execute it.

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

Now, when users execute the `/hello` command, the plugin will output `Hello world!`.

## 6. Compile and Run Plugin

After completing the plugin development, we need to compile it to JavaScript and then run it.

```bash
npm run build
```

After compilation, we can find the compiled plugin files in the `dist` directory. We put the `dist` folder in the `plugins` folder and then run the server.

:::tip
Of course, you can also rename `dist` to your plugin name for better management.
:::

```bash
endstone
```

After the server is running, we can enter the `/hello` command in the console and see the output `Hello world!`.

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

## 7. Publish Your Plugin

After completing plugin development and testing, you can publish your plugin to the plugin marketplace for other users to use.
