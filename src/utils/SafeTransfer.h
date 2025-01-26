#pragma once
#include "utils/Using.h"
#include <memory>


namespace jse {


template <typename T>
struct SafeTransfer {
    ScriptEngine*     mEngine;
    script::Global<T> mGlobal;

    SafeTransfer(const SafeTransfer&)            = delete; // copy constructor
    SafeTransfer& operator=(const SafeTransfer&) = delete; // copy assignment operator

    SafeTransfer(SafeTransfer&&) noexcept            = default; // move constructor
    SafeTransfer& operator=(SafeTransfer&&) noexcept = default; // move assignment operator

    explicit SafeTransfer(ScriptEngine* engine, script::Global<T> global)
    : mEngine(engine),
      mGlobal(std::move(global)) {}

    [[nodiscard]] static std::shared_ptr<SafeTransfer<T>> make(ScriptEngine* engine, script::Global<T> global) {
        return std::make_shared<SafeTransfer<T>>(engine, std::move(global));
    }
};


} // namespace jse