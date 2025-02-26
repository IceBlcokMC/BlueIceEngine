#pragma once
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-external.h"
#include "v8-local-handle.h"
#include "v8-value.h"

namespace v8_util {

// 定义只读的全局变量
inline void DefineReadOnlyGlobal(v8::Isolate* isolate, const char* name, v8::Local<v8::Value> value) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object>  global  = context->Global();

    v8::Local<v8::String> key = v8::String::NewFromUtf8(isolate, name).ToLocalChecked();

    auto attrs =
        static_cast<v8::PropertyAttribute>(v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete);

    global->DefineOwnProperty(context, key, value, attrs).Check();
}

// 获取全局变量
[[nodiscard]] inline v8::Local<v8::Value> GetGlobalVariable(v8::Isolate* isolate, const char* name) {
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::Context>   context = isolate->GetCurrentContext();
    v8::Local<v8::Object>    global  = context->Global();

    v8::Local<v8::String> key = v8::String::NewFromUtf8(isolate, name).ToLocalChecked();

    return handle_scope.Escape(global->Get(context, key).ToLocalChecked());
}


} // namespace v8_util
