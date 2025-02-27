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
    EngineID                                      id_{};
    std::unique_ptr<node::CommonEnvironmentSetup> commonEnvPtr_;
    bool                                          isRunning_{false};
    bool                                          isDestroying_{false};
    std::string                                   entryPoint_; // 入口文件 package main

    puerts::FCppObjectMapper* cppMapper_{}; // cpp对象映射器

    std::unique_ptr<JavaScriptPluginBuilder> pluginDescriptionBuilder_; // 插件描述构建器(在插件实例创建后释放)
    endstone::Plugin*                        pluginPtr_{nullptr};       // 分配的插件实例

    v8::Global<v8::Function> onLoad_;
    v8::Global<v8::Function> onEnable_;
    v8::Global<v8::Function> onDisable_;
    v8::Global<v8::Function> onCommand_;

public:
    [[nodiscard]] v8::Isolate*           isolate() const { return commonEnvPtr_->isolate(); }
    [[nodiscard]] v8::Local<v8::Context> context() const { return commonEnvPtr_->context(); }
    [[nodiscard]] v8::Local<v8::Object>  global() const { return commonEnvPtr_->context()->Global(); }
    [[nodiscard]] node::Environment*     env() const { return commonEnvPtr_->env(); }

public:
    explicit V8Engine(EngineID id, std::unique_ptr<node::CommonEnvironmentSetup> envs)
    : id_(id),
      commonEnvPtr_(std::move(envs)) {}

    V8Engine(const V8Engine&)            = delete;
    V8Engine& operator=(const V8Engine&) = delete;
    V8Engine(V8Engine&&)                 = delete;
    V8Engine& operator=(V8Engine&&)      = delete;
};

using V8EnginePtr = std::unique_ptr<V8Engine>;


} // namespace jse