# 快速开始

## 开始之前

在开始之前，请确保您已安装好了 [endstone](https://github.com/EndstoneMC/endstone)。

## 下载 Js_Engine

> 🚧 Js_Engine 目前处于开发阶段，请前往 [Actions](https://github.com/IceBlcokMC/Js_Engine/actions/workflows/build.yml) 查看最新构建。

目前我们提供了以下构建版本：

| file                                                                | size    |
| :------------------------------------------------------------------ | :------ |
| Js_Engine-Linux-full-x64-1ce1360d13a2d89d06a091f7196dce12d7caa0b5   | 54.9 MB |
| Js_Engine-Linux-x64-1ce1360d13a2d89d06a091f7196dce12d7caa0b5        | 6.48 MB |
| Js_Engine-Windows-full-x64-1ce1360d13a2d89d06a091f7196dce12d7caa0b5 | 41 MB   |
| Js_Engine-Windows-x64-1ce1360d13a2d89d06a091f7196dce12d7caa0b5      | 3.73 MB |

其中, `full` 版本包含所有依赖项(包括 `libnode` 和 `npm`)

我们以 `windows` 为例, 下载 `Js_Engine-windows-2022-full-x64-6feb3be0b10a10d88e3e7cadd7041f31ed9e0c57`

## 安装 Js_Engine

打开我们刚刚下载的 `Js_Engine` 压缩包，我们可以看到以下目录结构：

```
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

打开已安装 `bedrock_server` 文件夹，将以上内容拖入 `bedrock_server` 文件夹中。

::: info
`libnode.dll` 和 `node.exe` 要和 `bedrock_server.exe` 在同一目录下
:::

返回上级目录，打开终端，执行 `endstone` 看到 `Js_Engine` 已经被加载代表安装成功。

## 安装 Js_Engine 插件

开发者分发的 `Js_Engine` 插件通常为压缩包，如：

```
helloworld.zip
└── helloworld
    ├── index.js
    └── package.json
```

我们将压缩包内整个 `helloworld` 文件夹解压到 `bds/plugins/` 下即可。
