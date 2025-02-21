#pragma once
#include "CppObjectMapper.h"
#include "utils/Using.h"
#include "v8-isolate.h"
#include <node.h>

namespace jse {


struct EngineWrapper {
    EngineID                                      mID{};
    std::unique_ptr<node::CommonEnvironmentSetup> mEnvSetup;
    bool                                          mIsRunning{false};
    bool                                          mIsDestroying{false};

    string                    mEntryPoint;
    puerts::FCppObjectMapper* mMapper{};

public:
    [[nodiscard]] v8::Isolate*           isolate() const { return mEnvSetup->isolate(); }
    [[nodiscard]] v8::Local<v8::Context> context() const { return mEnvSetup->context(); }
    [[nodiscard]] v8::Local<v8::Object>  global() const { return mEnvSetup->context()->Global(); }
    [[nodiscard]] node::Environment*     env() const { return mEnvSetup->env(); }

public:
    explicit EngineWrapper(EngineID id, std::unique_ptr<node::CommonEnvironmentSetup> envs)
    : mID(id),
      mEnvSetup(std::move(envs)) {}
};

using EngineWrapperPtr = std::unique_ptr<EngineWrapper>;


} // namespace jse