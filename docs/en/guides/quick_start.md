# Quick Start

## Before You Start

Before you start, please make sure you have installed [endstone](https://github.com/EndstoneMC/endstone).

## Download Js_Engine

Currently, we provide the following build versions:

| file                           | size    |
| :----------------------------- | :------ |
| Js_Engine-Linux-full-x64.zip   | 54.9 MB |
| Js_Engine-Linux-x64.zip        | 6.48 MB |
| Js_Engine-Windows-full-x64.zip | 41 MB   |
| Js_Engine-Windows-x64.zip      | 3.73 MB |

The `full` version includes all dependencies (including `libnode` and `npm`).

We will take `windows` as an example, download `Js_Engine-windows-full-x64.zip`.

## Install Js_Engine

Open the `Js_Engine` zip file we just downloaded, we can see the following directory structure:

```plaintext
Js_Engine-windows-x64.zip
├── plugins
|   ├── js_engine
│   │   ├── types
│   │   └── node_modules
│   ├── js_engine.dll
│   └── js_engine.pdb
├── libnode.dll
└── node.exe
```

Open the `bedrock_server` folder that has been installed, drag the above content into the `bedrock_server` folder.

::: info
`libnode.dll` and `node.exe` need to be in the same directory as `bedrock_server.exe`
:::

Return to the parent directory, open the terminal, execute `endstone` to see that Js_Engine has been loaded, indicating successful installation.

## Install Js_Engine Plugins

Developers' distributed Js_Engine plugins are usually compressed packages, such as:

```
helloworld.zip
└── helloworld
    ├── index.js
    └── package.json
```

We can extract the entire `helloworld` folder to `bds/plugins/` folder.
