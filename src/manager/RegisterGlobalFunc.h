#pragma once
#include "Entry.h"
#include "JSClassRegister.h"
#include "ScriptBackend.hpp"
#include "TypeInfo.hpp"
#include "endstone/plugin/plugin.h"
#include "loader/JavaScriptPluginBuilder.h"
#include "manager/NodeManager.h"
#include "manager/V8Engine.h"
#include "puerts_impl/DeclarationGenerator.h"
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
        builder->load = vObj["load"].As<endstone::PluginLoadOrder>();
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
        builder->default_permission = vObj["default_permission"].As<endstone::PermissionDefault>();
    }

    // TODO: permission
    // TODO: commands

    if (vObj.HasKey("onLoad")) {
        auto onLoad = vObj["onLoad"].GetValue();
        if (!onLoad->IsFunction()) {
            Entry::getInstance()->getLogger().error("onLoad is not a function");
        }
        engine->onLoad_ = v8::Global<v8::Function>{isolate, onLoad.As<v8::Function>()};
    }
    if (vObj.HasKey("onEnable")) {
        auto onEnable = vObj["onEnable"].GetValue();
        if (!onEnable->IsFunction()) {
            Entry::getInstance()->getLogger().error("onEnable is not a function");
        }
        engine->onEnable_ = v8::Global<v8::Function>{isolate, onEnable.As<v8::Function>()};
    }
    if (vObj.HasKey("onDisable")) {
        auto onDisable = vObj["onDisable"].GetValue();
        if (!onDisable->IsFunction()) {
            Entry::getInstance()->getLogger().error("onDisable is not a function");
        }
        engine->onDisable_ = v8::Global<v8::Function>{isolate, onDisable.As<v8::Function>()};
    }
    if (vObj.HasKey("onCommand")) {
        auto onCommand = vObj["onCommand"].GetValue();
        if (!onCommand->IsFunction()) {
            Entry::getInstance()->getLogger().error("onCommand is not a function");
        }
        engine->onCommand_ = v8::Global<v8::Function>{isolate, onCommand.As<v8::Function>()};
    }

    engine->pluginDescriptionBuilder_ = std::move(builder);
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
    v8_exception::check(vtry);

    if (!engine) {
        Entry::getInstance()->getLogger().error("Engine not found");
        args.GetReturnValue().Set(v8::Null(isolate));
        return;
    }

    auto pluginPtr = engine->pluginPtr_;
    if (!pluginPtr) {
        Entry::getInstance()->getLogger().error("Plugin instance is null");
        args.GetReturnValue().Set(v8::Null(isolate));
        return;
    }

    auto class_ = puerts::FindClassByID(puerts::StaticTypeId<endstone::Plugin>::get());
    if (!class_) {
        Entry::getInstance()->getLogger().error("endstone::Plugin class not found");
        args.GetReturnValue().Set(v8::Null(isolate));
        return;
    }

    auto res = engine->cppMapper_
                   ->FindOrAddCppObject(isolate, ctx, puerts::StaticTypeId<endstone::Plugin>::get(), pluginPtr, true);

    v8_exception::check(vtry);
    args.GetReturnValue().Set(res);
}

inline void Js_GetDeclaration(v8::FunctionCallbackInfo<v8::Value> const& args) {
    puerts::DeclarationGenerator dg;

    puerts::ForeachRegisterClass([&](const puerts::JSClassDefinition* classDefinition) {
        if (classDefinition->TypeId && classDefinition->ScriptName) {
            dg.GenClass(classDefinition);
        }
    });

    args.GetReturnValue().Set(
        v8::String::NewFromUtf8(args.GetIsolate(), dg.GetOutput().c_str(), v8::NewStringType::kNormal).ToLocalChecked()
    );
}

inline void Js_LoadNativeClass(v8::FunctionCallbackInfo<v8::Value> const& args) {
    auto pom = static_cast<puerts::FCppObjectMapper*>((v8::Local<v8::External>::Cast(args.Data()))->Value());
    pom->LoadCppType(args);
}

inline void RegisterEngineApi(V8Engine* engine) {
    auto isolate = v8::Isolate::GetCurrent();

    v8::Locker         locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope    handle_scope(isolate);

    auto               ctx = isolate->GetCurrentContext();
    v8::Context::Scope context_scope(ctx);

    auto tpl = v8::ObjectTemplate::New(isolate);

    // Engine.registerPlugin
    tpl->Set(
        v8::String::NewFromUtf8Literal(isolate, "registerPlugin"),
        v8::FunctionTemplate::New(isolate, Js_RegisterPlugin)
    );

    // Engine.getSelf
    tpl->Set(v8::String::NewFromUtf8Literal(isolate, "getSelf"), v8::FunctionTemplate::New(isolate, Js_GetSelf));

    // Engine.loadNativeClass
    tpl->Set(
        v8::String::NewFromUtf8Literal(isolate, "getDeclaration"),
        v8::FunctionTemplate::New(isolate, Js_GetDeclaration)
    );

    // Engine.loadNativeClass
    tpl->Set(
        v8::String::NewFromUtf8Literal(isolate, "loadNativeClass"),
        v8::FunctionTemplate::New(isolate, Js_LoadNativeClass, v8::External::New(isolate, engine->cppMapper_))
    );

    ctx->Global()
        ->Set(ctx, v8::String::NewFromUtf8Literal(isolate, "Engine"), tpl->NewInstance(ctx).ToLocalChecked())
        .Check();
}

inline void RegisterGlobalFunc(V8Engine* engine) {
    auto isolate = engine->isolate();

    v8_util::DefineReadOnlyGlobal(isolate, "__ENGINE_ID__", v8::Number::New(isolate, static_cast<double>(engine->id_)));

    RegisterEngineApi(engine);
}


} // namespace jse