#pragma once
#include "CppObjectMapper.h"
#include "endstone/plugin/plugin.h"
#include "loader/JavaScriptPluginBuilder.h"
#include "v8-function.h"
#include "v8-isolate.h"
#include "v8-persistent-handle.h"
#include <memory>
#include <node.h>
#include <string>

namespace jse {

using EngineID = uint64_t;

struct V8Engine {
    EngineID                                      mID{};
    std::unique_ptr<node::CommonEnvironmentSetup> mEnvSetup;
    bool                                          mIsRunning{false};
    bool                                          mIsDestroying{false};
    std::string                                   mEntryPoint; // 入口文件 package main

    puerts::FCppObjectMapper* mCppMapper{}; // cpp对象映射器

    std::unique_ptr<JavaScriptPluginBuilder> mPluginDescriptionBuilder; // 插件描述构建器(在插件实例创建后释放)
    endstone::Plugin*                        mPluginInstance{nullptr};  // 分配的插件实例

    v8::Global<v8::Function> mOnLoadFunc;
    v8::Global<v8::Function> mOnEnableFunc;
    v8::Global<v8::Function> mDisableFunc;
    v8::Global<v8::Function> mOnCommandFunc;

public:
    [[nodiscard]] v8::Isolate*           isolate() const { return mEnvSetup->isolate(); }
    [[nodiscard]] v8::Local<v8::Context> context() const { return mEnvSetup->context(); }
    [[nodiscard]] v8::Local<v8::Object>  global() const { return mEnvSetup->context()->Global(); }
    [[nodiscard]] node::Environment*     env() const { return mEnvSetup->env(); }

public:
    explicit V8Engine(EngineID id, std::unique_ptr<node::CommonEnvironmentSetup> envs)
    : mID(id),
      mEnvSetup(std::move(envs)) {}

    V8Engine(const V8Engine&)            = delete;
    V8Engine& operator=(const V8Engine&) = delete;
    V8Engine(V8Engine&&)                 = delete;
    V8Engine& operator=(V8Engine&&)      = delete;
};

using V8EnginePtr = std::unique_ptr<V8Engine>;


} // namespace jse