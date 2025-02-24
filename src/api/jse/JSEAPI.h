#pragma once
#include "ScriptBackend.hpp"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/plugin/plugin_load_order.h"
#include "puerts_impl/ConverterImpl.h"
#include "puerts_impl/EnumImpl.h"
#include "utils/Using.h"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-function.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-primitive.h"
#include "v8-value.h"
#include "v8_utils/Converter.h"
#include "v8_utils/V8Object.h"
#include <functional>
#include <string>
#include <utility>


class NativeBuilder : public endstone::detail::PluginDescriptionBuilder {
public:
    std::string name;
    std::string version;

    using Function = std::function<void()>;
    Function             onLoad;
    Function             onEnable;
    Function             onDisable;
    v8::Local<v8::Value> commands_;
    v8::Local<v8::Value> permissions_;

    explicit NativeBuilder(std::string name, std::string version)
    : name(std::move(name)),
      version(std::move(version)) {}

    endstone::PluginDescription build() {
        return endstone::detail::PluginDescriptionBuilder::build(std::move(name), std::move(version));
    }
};

class JSEAPI {
public:
    bool registerPlugin(NativeBuilder* builder) { return false; }

    // endstone::Plugin* getSelf() { return nullptr; }

    static bool isWindows() {
#if defined(_WIN32) || defined(WIN32)
        return true;
#else
        return false;
#endif
    }

    static bool isLinux() {
#if defined(__linux__)
        return true;
#else
        return false;
#endif
    }
};


// 为 std::vector<T> 特化转换器

UsingCppType(JSEAPI);
UsingCppType(NativeBuilder);
UsingNamedCppType(endstone::detail::PluginDescriptionBuilder, "PluginDescriptionBuilder");
// UsingNamedCppType(endstone::PluginDescription, "PluginDescription");
UsingNamedCppEnum(endstone::PermissionDefault, "PermissionDefault");
UsingNamedCppEnum(endstone::PluginLoadOrder, "PluginLoadOrder");


void RegisterNativeBuilder() {
    puerts::DefineClass<endstone::detail::PluginDescriptionBuilder>()
        .Property("description", MakeProperty(&endstone::detail::PluginDescriptionBuilder::description))
        .Property("load", MakeProperty(&endstone::detail::PluginDescriptionBuilder::load))
        .Property("authors", MakeProperty(&endstone::detail::PluginDescriptionBuilder::authors))
        .Property("contributors", MakeProperty(&endstone::detail::PluginDescriptionBuilder::contributors))
        .Property("website", MakeProperty(&endstone::detail::PluginDescriptionBuilder::website))
        .Property("prefix", MakeProperty(&endstone::detail::PluginDescriptionBuilder::prefix))
        .Property("provides", MakeProperty(&endstone::detail::PluginDescriptionBuilder::provides))
        .Property("depend", MakeProperty(&endstone::detail::PluginDescriptionBuilder::depend))
        .Property("soft_depend", MakeProperty(&endstone::detail::PluginDescriptionBuilder::soft_depend))
        .Property("load_before", MakeProperty(&endstone::detail::PluginDescriptionBuilder::load_before))
        // .Property("default_permission",
        // MakeProperty(&endstone::detail::PluginDescriptionBuilder::default_permission))
        .Register();

    puerts::DefineClass<NativeBuilder>()
        .Constructor<string, string>()
        .Extends<endstone::detail::PluginDescriptionBuilder>()
        .Property("name", MakeProperty(&NativeBuilder::name))
        .Property("version", MakeProperty(&NativeBuilder::version))
        // .Property("commands", MakeProperty(&NativeBuilder::commands_))
        // .Property("permissions", MakeProperty(&NativeBuilder::permissions_))
        .Register();
}

void RegisterJSEAPI() {
    puerts::DefineClass<JSEAPI>()
        .Function("registerPlugin", MakeCheckFunction(&JSEAPI::registerPlugin))
        // .Function("getSelf", MakeCheckFunction(&JSEAPI::getSelf))
        .Function("isWindows", MakeCheckFunction(&JSEAPI::isWindows))
        .Function("isLinux", MakeCheckFunction(&JSEAPI::isLinux))
        .Register();
}
