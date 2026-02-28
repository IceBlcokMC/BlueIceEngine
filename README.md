# BlueIceEngine - A JavaScript runtime for endstone

> In a previous life as a Js_Engine, you can find the previous source code
> here [Go](https://github.com/IceBlcokMC/Js_Engine_v1).

## Introduction

// TODO

## Example

- bedrock_server/plugins/test_bie/
    - index.js
    - package.json

```js
console.log("Script loaded");

let plugin = null;

let desc = new PluginDescription(
    "test_js_plugin",
    "0.0.0",
    "Test JS Plugin",
    PluginLoadOrder.PostWorld,
    [],
    [],
    "",
    "",
    [],
    [],
    [],
    [],
    /* PermissionDefault.a */ 2,
    [],
    [],
);

export default class TestPlugin extends Plugin {
    constructor() {
        super();
        plugin = this;
    }

    onLoad() {
        console.log("onLoad called");
        console.log("dataFloder: ", this.dataFolder)
    }

    onEnable() {
        console.log("onEnable called");
    }

    onDisable() {
        console.log("onDisable called");
    }

    getDescription() {
        return desc;
    }
}
```