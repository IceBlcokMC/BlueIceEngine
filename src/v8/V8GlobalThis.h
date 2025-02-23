#pragma once
#include "v8-context.h"
#include "v8-function.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-persistent-handle.h"
#include "v8-value.h"


namespace jse::V8GlobalThis {

[[nodiscard]] inline v8::Local<v8::Value>
Get(v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::string const& key) {
    return ctx->Global()->Get(ctx, v8::String::NewFromUtf8(isolate, key.c_str()).ToLocalChecked()).ToLocalChecked();
}

inline void Set(v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::string const& key, v8::Local<v8::Value> value) {
    ctx->Global()->Set(ctx, v8::String::NewFromUtf8(isolate, key.c_str()).ToLocalChecked(), value).FromJust();
}

inline void Delete(v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::string const& key) {
    ctx->Global()->Delete(ctx, v8::String::NewFromUtf8(isolate, key.c_str()).ToLocalChecked()).FromJust();
}

} // namespace jse::V8GlobalThis
