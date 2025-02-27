#pragma once
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-external.h"
#include "v8-local-handle.h"
#include "v8-primitive.h"
#include "v8-script.h"
#include "v8-value.h"
#include "v8_utils/V8Exception.h"

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

inline v8::Local<v8::Value> EvalJsCode(v8::Isolate* isoalte, const char* code) {
    v8::TryCatch vtry(isoalte);

    auto ctx = isoalte->GetCurrentContext();

    auto codeString = v8::String::NewFromUtf8(isoalte, code).ToLocalChecked();
    auto script     = v8::Script::Compile(ctx, codeString);
    auto result     = script.ToLocalChecked()->Run(ctx).ToLocalChecked();
    jse::v8_exception::check(vtry);
    return result;
}


} // namespace v8_util
