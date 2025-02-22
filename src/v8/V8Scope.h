#pragma once
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include <exception>
#include <string>


namespace jse {


class EnterV8Scope {
    v8::Locker         locker_;
    v8::Isolate::Scope isolateScope_;
    v8::HandleScope    handleScope_;
    v8::Context::Scope contextScope_;

public:
    explicit EnterV8Scope(v8::Isolate* isolate, v8::Local<v8::Context> context)
    : locker_(isolate),
      isolateScope_(isolate),
      handleScope_(isolate),
      contextScope_(context) {}
};

class ExitV8Scope {
    v8::Unlocker locker_;

public:
    explicit ExitV8Scope(v8::Isolate* isolate) : locker_(isolate) {}
};

class HandleV8Scope {
    v8::EscapableHandleScope handleScope_;

public:
    explicit HandleV8Scope(v8::Isolate* isolate) : handleScope_(isolate) {}

    template <typename T>
    v8::Local<v8::Value> Escape(v8::Local<T> value) {
        return handleScope_.Escape(value);
    }
};


class v8_exception : public std::exception {
    std::string message_;

public:
    explicit v8_exception(std::string message) : message_(std::move(message)) {}

public:
    [[nodiscard]] const char* what() const noexcept override {}

    [[nodiscard]] std::string message() const noexcept {}

    [[nodiscard]] std::string stackTrace() const noexcept {}
};


inline void CheckTryCatch(v8::TryCatch& tryCatch) {
    if (tryCatch.HasCaught()) {
        // 抛出异常
    }
}


} // namespace jse
