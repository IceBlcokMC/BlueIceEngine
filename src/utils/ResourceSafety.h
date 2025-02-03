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


template <typename T>
struct SafePointerHolder {
    T*                 mRawPtr{nullptr};
    std::unique_ptr<T> mUniquePtr{nullptr};

    /* SDK */
    explicit SafePointerHolder<T>(T* ptr) : mRawPtr(ptr) {}

    /* Js New */
    explicit SafePointerHolder<T>(std::unique_ptr<T> ptr) : mUniquePtr(std::move(ptr)) {}


    [[nodiscard]] T* get() const { return mRawPtr ? mRawPtr : mUniquePtr.get(); }

    [[nodiscard]] T copy() const { return *get(); }

    T* operator->() const { return get(); }

    operator bool() const { return mRawPtr || mUniquePtr; }

    SafeTransfer<T>& operator=(T* ptr) {
        mRawPtr = ptr;
        mUniquePtr.reset();
        return *this;
    }

    SafeTransfer<T>& operator=(std::unique_ptr<T> ptr) {
        mRawPtr    = nullptr;
        mUniquePtr = std::move(ptr);
        return *this;
    }
};


} // namespace jse