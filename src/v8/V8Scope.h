#pragma once
#include "manager/V8Engine.h"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"


namespace jse {


class EnterV8Scope final {
    v8::Locker         locker_;
    v8::Isolate::Scope isolateScope_;
    v8::HandleScope    handleScope_;
    v8::Context::Scope contextScope_;

public:
    explicit EnterV8Scope(V8Engine* engine)
    : locker_(engine->isolate()),
      isolateScope_(engine->isolate()),
      handleScope_(engine->isolate()),
      contextScope_(engine->context()) {}

    EnterV8Scope(const EnterV8Scope&)            = delete;
    EnterV8Scope& operator=(const EnterV8Scope&) = delete;
    EnterV8Scope(EnterV8Scope&&)                 = delete;
    EnterV8Scope& operator=(EnterV8Scope&&)      = delete;
};

class ExitV8Scope final {
    v8::Unlocker locker_;

public:
    explicit ExitV8Scope(v8::Isolate* isolate) : locker_(isolate) {}

    ExitV8Scope(const ExitV8Scope&)            = delete;
    ExitV8Scope& operator=(const ExitV8Scope&) = delete;
    ExitV8Scope(ExitV8Scope&&)                 = delete;
    ExitV8Scope& operator=(ExitV8Scope&&)      = delete;
};

class HandleV8Scope final {
    v8::EscapableHandleScope handleScope_;

public:
    explicit HandleV8Scope(v8::Isolate* isolate) : handleScope_(isolate) {}

    HandleV8Scope(const HandleV8Scope&)            = delete;
    HandleV8Scope& operator=(const HandleV8Scope&) = delete;
    HandleV8Scope(HandleV8Scope&&)                 = delete;
    HandleV8Scope& operator=(HandleV8Scope&&)      = delete;

    template <typename T>
    v8::Local<v8::Value> Escape(v8::Local<T> value) {
        return handleScope_.Escape(value);
    }
};


} // namespace jse
