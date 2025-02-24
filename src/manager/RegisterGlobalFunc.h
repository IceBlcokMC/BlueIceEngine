#pragma once
#include "DeclarationGenerator.h"
#include "JSClassRegister.h"
#include "manager/V8Engine.h"
#include "v8-context.h"
#include "v8-external.h"
#include "v8-function-callback.h"
#include "v8-local-handle.h"
#include "v8-primitive.h"
#include "v8-template.h"
#include "v8-value.h"
#include <string>

namespace jse {

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


} // namespace jse