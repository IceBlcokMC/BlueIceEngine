#pragma once
#include "DeclarationGenerator.h"
#include "JSClassRegister.h"
#include "ScriptBackend.hpp"
#include "V8Backend.hpp"
#include "endstone/command/command_executor.h"
#include "endstone/plugin/plugin.h"
#include "manager/NodeManager.h"
#include "manager/V8Engine.h"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-external.h"
#include "v8-function-callback.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include "v8-primitive.h"
#include "v8-template.h"
#include "v8-value.h"
#include "v8_utils/V8Exception.h"
#include "v8_utils/V8Util.h"
#include <iostream>
#include <string>


UsingCppType(endstone::Plugin);
UsingCppType(endstone::CommandExecutor);

struct AutoBinding {
    AutoBinding() {
        puerts::DefineClass<endstone::CommandExecutor>().Register();

        puerts::DefineClass<endstone::Plugin>()
            .Extends<endstone::CommandExecutor>()
            // .Method("getDescription", &endstone::Plugin::getDescription)
            .Method("onLoad", MakeFunction(&endstone::Plugin::onLoad))
            .Method("onEnable", MakeFunction(&endstone::Plugin::onEnable))
            .Method("onDisable", MakeFunction(&endstone::Plugin::onDisable))
            .Method("getName", MakeFunction(&endstone::Plugin::getName))
            .Register();
    }
};
AutoBinding __AutoBinding__;


namespace jse {


inline void Js_RegisterPlugin(const v8::FunctionCallbackInfo<v8::Value>& args) {}

inline void Js_GetPlugin(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);

    auto ctx = isolate->GetCurrentContext();

    v8::Context::Scope context_scope(ctx);
    v8::TryCatch       vtry{isolate};

    auto _IdVal    = ctx->Global()->Get(ctx, v8::String::NewFromUtf8Literal(isolate, "__ENGINE_ID__")).ToLocalChecked();
    auto _DoubleId = _IdVal->ToNumber(ctx).ToLocalChecked()->Value();
    auto id        = static_cast<EngineID>(_DoubleId);

    std::cout << "GetEngine id: " << id << std::endl;

    auto engine = NodeManager::getInstance().getEngine(id);
    v8_exception::checkTryCatch(vtry);
}

inline void RegisterEngineApi() {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Locker         locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);

    auto               ctx = v8::Context::New(isolate);
    v8::Context::Scope context_scope(ctx);

    // Engine.registerPlugin()
    // Engine.getSelf()
    auto tpl = v8::ObjectTemplate::New(isolate);
    tpl->Set(
        v8::String::NewFromUtf8Literal(isolate, "registerPlugin"),
        v8::FunctionTemplate::New(isolate, Js_RegisterPlugin)
    );

    tpl->Set(v8::String::NewFromUtf8Literal(isolate, "getSelf"), v8::FunctionTemplate::New(isolate, Js_GetPlugin));

    tpl->NewInstance(ctx).ToLocalChecked();
    ctx->Global()
        ->Set(ctx, v8::String::NewFromUtf8Literal(isolate, "Engine"), tpl->NewInstance(ctx).ToLocalChecked())
        .Check();
}

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

    v8_util::DefineReadOnlyGlobal(
        isolate,
        "__ENGINE_ID__",
        v8::Number::New(isolate, static_cast<double>(wrapper->mID))
    );

    RegisterEngineApi();
}


} // namespace jse