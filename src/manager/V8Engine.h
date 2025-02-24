#pragma once
#include "CppObjectMapper.h"
#include "v8-function.h"
#include "v8-isolate.h"
#include "v8-persistent-handle.h"
#include <node.h>

namespace jse {

using EngineID = uint64_t;

struct V8Engine {
    EngineID                                      mID{};
    std::unique_ptr<node::CommonEnvironmentSetup> mEnvSetup;
    bool                                          mIsRunning{false};
    bool                                          mIsDestroying{false};
    std::string                                   mEntryPoint; // 入口文件 package main
    puerts::FCppObjectMapper*                     mCppMapper{};

    v8::Global<v8::Function> mOnLoad;
    v8::Global<v8::Function> mOnEnable;
    v8::Global<v8::Function> mDisable;

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