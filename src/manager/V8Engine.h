#pragma once
#include "CppObjectMapper.h"
#include "utils/Using.h"
#include "v8-isolate.h"
#include <node.h>

namespace jse {


struct V8Engine {
    EngineID                                      mID{};
    std::unique_ptr<node::CommonEnvironmentSetup> mEnvSetup;
    bool                                          mIsRunning{false};
    bool                                          mIsDestroying{false};
    string                                        mEntryPoint; // 入口文件 package main
    puerts::FCppObjectMapper*                     mCppMapper{};

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