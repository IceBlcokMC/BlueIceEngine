#pragma once
#include "pch.h"

#include <memory>

namespace bie {

using VMID = uint64_t;

struct VM {
    using V8Engine = std::unique_ptr<v8kit::Engine>;
    using NodeEnv  = std::unique_ptr<node::CommonEnvironmentSetup>;

    VMID const id_;
    V8Engine   engine_;
    NodeEnv    nodeEnv_;

    VM(VM const&)            = delete;
    VM& operator=(VM const&) = delete;

    explicit VM(VMID id, V8Engine engine, NodeEnv nodeEnv)
    : id_(id),
      engine_(std::move(engine)),
      nodeEnv_(std::move(nodeEnv)) {}

    inline operator v8kit::Engine*() const { return engine_.get(); }
    inline operator const v8kit::Engine*() const { return engine_.get(); }
};


} // namespace bie