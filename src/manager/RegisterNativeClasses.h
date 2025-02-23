#pragma once
#include "DataTransfer.h"
#include "DeclarationGenerator.h"
#include "JSClassRegister.h"
#include "TypeInfo.hpp"
#include "api/jse/JSEAPI.h"
#include "manager/V8Engine.h"
#include "pesapi.h"
#include "utils/Using.h"
#include "v8-context.h"
#include "v8-external.h"
#include "v8-function-callback.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include "v8-primitive.h"
#include "v8-template.h"
#include "v8-value.h"
#include <set>
#include <sstream>
#include <string>


namespace jse {

// template <typename T>
// inline v8::Local<v8::Value> ConvertNativeInstToJsInst(V8Engine* wrapper, T* nativeInst) {
//     auto isolate = wrapper->isolate();
//     v8::Locker             lock(isolate);
//     v8::Isolate::Scope     isolate_scope(isolate);
//     v8::HandleScope        handle_scope(isolate);
//     v8::Local<v8::Context> context = wrapper->context();
//     v8::Context::Scope     context_scope(context);
//     if (!nativeInst) {
//         return v8::Null(isolate);
//     }
//     return puerts::DataTransfer::FindOrAddCData(
//         isolate,
//         context,
//         puerts::StaticTypeId<T>::get(), // 获取类型id
//         nativeInst,                     // Native实例
//         true                            // PassByPointer=true, 表示已存在的实例
//     );
// }

inline void RegisterGlobalFunc(V8Engine* wrapper) {
    auto isolate = wrapper->isolate();
    auto ctx     = wrapper->context();
    auto global  = ctx->Global();

    global
        ->Set(
            ctx,
            v8::String::NewFromUtf8(isolate, "loadNativeClass").ToLocalChecked(),
            v8::FunctionTemplate::New(
                isolate,
                [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                    auto pom =
                        static_cast<puerts::FCppObjectMapper*>((v8::Local<v8::External>::Cast(info.Data()))->Value());
                    pom->LoadCppType(info);
                },
                v8::External::New(isolate, wrapper->mCppMapper)
            )
                ->GetFunction(ctx)
                .ToLocalChecked()
        )
        .Check();

    global
        ->Set(
            ctx,
            v8::String::NewFromUtf8(isolate, "__declaration__").ToLocalChecked(),
            v8::FunctionTemplate::New(
                isolate,
                [](const v8::FunctionCallbackInfo<v8::Value>& info) {
                    puerts::DeclarationGenerator dg;

                    puerts::ForeachRegisterClass([&](const puerts::JSClassDefinition* classDefinition) {
                        if (classDefinition->TypeId && classDefinition->ScriptName) {
                            dg.GenClass(classDefinition);
                        }
                    });

                    info.GetReturnValue().Set(
                        v8::String::NewFromUtf8(info.GetIsolate(), dg.GetOutput().c_str(), v8::NewStringType::kNormal)
                            .ToLocalChecked()
                    );
                }
            )
                ->GetFunction(ctx)
                .ToLocalChecked()
        )
        .Check();

    global
        ->Set(
            ctx,
            v8::String::NewFromUtf8(isolate, "__ENGINE_ID__").ToLocalChecked(),
            v8::Number::New(isolate, static_cast<double>(wrapper->mID))
        )
        .Check();
}

inline void RegisterNativeClasses(V8Engine* wrapper) {
    RegisterGlobalFunc(wrapper);

    RegisterJSEAPI();
}


} // namespace jse