#pragma once
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include <exception>
#include <string>

namespace jse {


class v8_exception : public std::exception {
    v8::Isolate*           isolate_;
    v8::Local<v8::Context> ctx_;
    std::string            message_;
    v8::Local<v8::Value>   exception_;

public:
    v8_exception(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Value> exception)
    : isolate_(isolate),
      ctx_(ctx),
      exception_(exception) {
        v8::TryCatch try_catch(isolate_);
        message_ = *v8::String::Utf8Value(isolate_, exception_);
    }

    [[nodiscard]] const char* what() const noexcept override { return message_.c_str(); }

    [[nodiscard]] v8::Local<v8::Value> exception() const noexcept { return exception_; }

    [[nodiscard]] std::string message() const noexcept { return message_; }

    [[nodiscard]] std::string stacktrace() const noexcept {
        v8::TryCatch try_catch(isolate_);
        auto         maybe_stack = v8::TryCatch::StackTrace(ctx_, exception_);
        if (maybe_stack.IsEmpty()) {
            return "No stack trace available";
        }
        auto stack = maybe_stack.ToLocalChecked();
        return *v8::String::Utf8Value(isolate_, stack);
    }


    /**
     * @brief 检查 v8::TryCatch 是否有异常，如果有则抛出 v8_exception
     * @warning 必须在 v8 作用域内调用
     */
    static void checkTryCatch(v8::TryCatch& vtry) {
        if (vtry.HasCaught()) {
            auto isolate   = v8::Isolate::GetCurrent();
            auto ctx       = isolate->GetCurrentContext();
            auto exception = vtry.Exception();
            throw v8_exception(isolate, ctx, exception);
        }
    }
};


} // namespace jse