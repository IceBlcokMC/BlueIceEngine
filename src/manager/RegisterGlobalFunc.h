#pragma once
#include "DeclarationGenerator.h"
#include "Entry.h"
#include "JSClassRegister.h"
#include "ScriptBackend.hpp"
#include "TypeInfo.hpp"
#include "V8Backend.hpp"
#include "endstone/command/command_executor.h"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_load_order.h"
#include "loader/JavaScriptPluginBuilder.h"
#include "manager/NodeManager.h"
#include "manager/V8Engine.h"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-external.h"
#include "v8-function-callback.h"
#include "v8-function.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-locker.h"
#include "v8-persistent-handle.h"
#include "v8-primitive.h"
#include "v8-template.h"
#include "v8-value.h"
#include "v8_utils/V8Exception.h"
#include "v8_utils/V8Object.h"
#include "v8_utils/V8Util.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>


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


inline void Js_RegisterPlugin(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);

    auto               ctx = isolate->GetCurrentContext();
    v8::Context::Scope context_scope(ctx);
    v8::TryCatch       vtry{isolate};

    if (args.Length() != 1 || !args[0]->IsObject()) {
        throw std::runtime_error("Invalid arguments");
    }

    auto engine = NodeManager::getInstance().getEngine(static_cast<EngineID>(
        v8_util::GetGlobalVariable(isolate, "__ENGINE_ID__")->ToNumber(ctx).ToLocalChecked()->Value()
    ));

    auto rawObject = args[0]->ToObject(ctx).ToLocalChecked()->ToObject(ctx).ToLocalChecked();

    auto     builder = std::make_unique<JavaScriptPluginBuilder>();
    V8Object vObj{isolate, ctx, rawObject};

    // 解析插件描述
    if (vObj.HasKey("name")) {
        builder->name = vObj["name"].As<std::string>();
    }
    if (vObj.HasKey("version")) {
        builder->version = vObj["version"].As<std::string>();
    }
    if (vObj.HasKey("description")) {
        builder->description = vObj["description"].As<std::string>();
    }
    if (vObj.HasKey("load")) {
        // builder->load = vObj["load"].As<endstone::PluginLoadOrder>();
    }
    if (vObj.HasKey("authors")) {
        builder->authors = vObj["authors"].As<std::vector<std::string>>();
    }
    if (vObj.HasKey("contributors")) {
        builder->contributors = vObj["contributors"].As<std::vector<std::string>>();
    }
    if (vObj.HasKey("website")) {
        builder->website = vObj["website"].As<std::string>();
    }
    if (vObj.HasKey("prefix")) {
        builder->prefix = vObj["prefix"].As<std::string>();
    }
    if (vObj.HasKey("provides")) {
        builder->provides = vObj["provides"].As<std::vector<std::string>>();
    }
    if (vObj.HasKey("depend")) {
        builder->depend = vObj["depend"].As<std::vector<std::string>>();
    }
    if (vObj.HasKey("soft_depend")) {
        builder->soft_depend = vObj["soft_depend"].As<std::vector<std::string>>();
    }
    if (vObj.HasKey("load_before")) {
        builder->load_before = vObj["load_before"].As<std::vector<std::string>>();
    }
    if (vObj.HasKey("default_permission")) {
        // builder->default_permission = vObj["default_permission"].As<endstone::PermissionDefault>();
    }

    // TODO: permission
    // TODO: commands

    if (vObj.HasKey("onLoad")) {
        auto onLoad = vObj["onLoad"].GetValue();
        if (!onLoad->IsFunction()) {
            Entry::getInstance()->getLogger().error("onLoad is not a function");
        }
        engine->mOnLoadFunc = v8::Global<v8::Function>{isolate, onLoad.As<v8::Function>()};
    }
    if (vObj.HasKey("onEnable")) {
        auto onEnable = vObj["onEnable"].GetValue();
        if (!onEnable->IsFunction()) {
            Entry::getInstance()->getLogger().error("onEnable is not a function");
        }
        engine->mOnEnableFunc = v8::Global<v8::Function>{isolate, onEnable.As<v8::Function>()};
    }
    if (vObj.HasKey("onDisable")) {
        auto onDisable = vObj["onDisable"].GetValue();
        if (!onDisable->IsFunction()) {
            Entry::getInstance()->getLogger().error("onDisable is not a function");
        }
        engine->mDisableFunc = v8::Global<v8::Function>{isolate, onDisable.As<v8::Function>()};
    }
    if (vObj.HasKey("onCommand")) {
        auto onCommand = vObj["onCommand"].GetValue();
        if (!onCommand->IsFunction()) {
            Entry::getInstance()->getLogger().error("onCommand is not a function");
        }
        engine->mOnCommandFunc = v8::Global<v8::Function>{isolate, onCommand.As<v8::Function>()};
    }

    engine->mPluginDescriptionBuilder = std::move(builder);
}

inline void Js_GetSelf(const v8::FunctionCallbackInfo<v8::Value>& args) {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);

    auto ctx = isolate->GetCurrentContext();

    v8::Context::Scope context_scope(ctx);
    v8::TryCatch       vtry{isolate};

    auto IDValue = v8_util::GetGlobalVariable(isolate, "__ENGINE_ID__");
    auto id      = static_cast<EngineID>(IDValue->ToNumber(ctx).ToLocalChecked()->Value());

    auto engine = NodeManager::getInstance().getEngine(id);
    v8_exception::checkTryCatch(vtry);

    if (!engine) {
        Entry::getInstance()->getLogger().error("Engine not found");
        args.GetReturnValue().Set(v8::Null(isolate));
        return;
    }

    auto class_ = puerts::FindClassByID(puerts::StaticTypeId<endstone::Plugin>::get());
    if (!class_) {
        Entry::getInstance()->getLogger().error("endstone::Plugin class not found");
        args.GetReturnValue().Set(v8::Null(isolate));
        return;
    }
    auto pluginPtr = engine->mPluginInstance;

    v8_exception::checkTryCatch(vtry);
    args.GetReturnValue().Set(engine->mCppMapper->GetTemplateOfClass(isolate, class_)->GetFunction(ctx).ToLocalChecked()
    );
}

inline void RegisterEngineApi() {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Locker         locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);

    auto               ctx = isolate->GetCurrentContext();
    v8::Context::Scope context_scope(ctx);

    // Engine.registerPlugin()
    // Engine.getSelf()
    auto tpl = v8::ObjectTemplate::New(isolate);
    tpl->Set(
        v8::String::NewFromUtf8Literal(isolate, "registerPlugin"),
        v8::FunctionTemplate::New(isolate, Js_RegisterPlugin)
    );

    tpl->Set(v8::String::NewFromUtf8Literal(isolate, "getSelf"), v8::FunctionTemplate::New(isolate, Js_GetSelf));

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