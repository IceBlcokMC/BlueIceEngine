#pragma once
#include "ScriptBackend.hpp"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/plugin/plugin_load_order.h"
#include "loader/JavaScriptPlugin.h"
#include "utils/Using.h"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-primitive.h"
#include <utility>
#include <vector>


template <typename T, typename Enable = void>
struct Converter {
    // toCpp
    static T toCpp(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) { return {}; }

    // toV8
    static v8::Local<v8::Value> toV8(v8::Isolate* isolate, v8::Local<v8::Context> ctx, T value) { return {}; }
};

// string
template <>
struct Converter<std::string> {
    static std::string toCpp(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        v8::HandleScope scope(isolate);
        v8::TryCatch    tryCatch(isolate);

        if (value->IsString()) {
            return *v8::String::Utf8Value(isolate, value);
        }
        return {};
    }
    static v8::Local<v8::Value> toV8(v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::string value) {
        v8::EscapableHandleScope scope(isolate);
        v8::TryCatch             tryCatch(isolate);

        return scope.Escape(v8::String::NewFromUtf8(isolate, value.c_str()).ToLocalChecked());
    }
};




class PluginRegister {
public:
    endstone::detail::PluginDescriptionBuilder builder;
    string                                     name;
    string                                     version;

    explicit PluginRegister(
        string                      name,
        string                      version,
        string                      description,
        endstone::PluginLoadOrder   loadOrder         = endstone::PluginLoadOrder::PostWorld,
        std::vector<string>         author            = {},
        std::vector<string>         contributors      = {},
        string                      website           = "",
        string                      prefix            = "",
        std::vector<string>         provides          = {},
        std::vector<string>         depends           = {},
        std::vector<string>         softDepends       = {},
        std::vector<string>         loadBefore        = {},
        endstone::PermissionDefault permissionDefault = endstone::PermissionDefault::Operator
    )
    : name(std::move(name)),
      version(std::move(version)) {
        builder.description        = std::move(description);
        builder.load               = loadOrder;
        builder.authors            = std::move(author);
        builder.contributors       = std::move(contributors);
        builder.website            = std::move(website);
        builder.prefix             = std::move(prefix);
        builder.provides           = std::move(provides);
        builder.depend             = std::move(depends);
        builder.soft_depend        = std::move(softDepends);
        builder.load_before        = std::move(loadBefore);
        builder.default_permission = permissionDefault;
    }

    /**
     * 注册权限
     * {
     *     "权限名": {
     *         "description": "权限描述",
     *         "default": "权限默认值" // endstone::PermissionDefault
     *     }
     * }
     */
    void registerPermissions(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> permissions) {
        v8::HandleScope scope(isolate);
        v8::TryCatch    tryCatch(isolate);

        try {
            auto maybe_keys = permissions->GetOwnPropertyNames(ctx);
            if (maybe_keys.IsEmpty()) {
                return;
            }
            auto keys   = maybe_keys.ToLocalChecked();
            int  length = keys->Length();

            for (int i = 0; i < length; i++) {
                auto maybe_key = keys->Get(ctx, i);
                if (maybe_key.IsEmpty()) {
                    continue;
                }

                auto key     = Converter<std::string>::toCpp(isolate, ctx, maybe_key.ToLocalChecked());
                auto builder = endstone::detail::PermissionBuilder(key);
            }
        } catch (...) {}
    }

    /**
     * 注册命令
     * {
     *     "cmd": {
     *         "description": "命令描述",
     *         "usages": ["/cmd"],
     *         "permission": ["权限名"],
     *     }
     * }
     */
    void registerCommands(v8::Local<v8::Object> commands) {}


    [[nodiscard]] endstone::PluginDescription build() { return builder.build(std::move(name), std::move(version)); }
};


class JSEAPI {
public:
    bool registerPlugin() { return false; }

    // Local<Value> JSEAPI::getSelf(Arguments const&) {
    //     try {
    //         if (!ENGINE_DATA()->mPlugin) {
    //             return Local<Value>();
    //         }
    //         return PluginAPI::newInstance(ENGINE_DATA()->mPlugin);
    //     }
    //     Catch;
    // }

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


UsingCppType(JSEAPI);

void RegisterJSEAPI() {
    puerts::DefineClass<JSEAPI>()
        .Function("registerPlugin", MakeCheckFunction(&JSEAPI::registerPlugin))
        .Function("isWindows", MakeCheckFunction(&JSEAPI::isWindows))
        .Function("isLinux", MakeCheckFunction(&JSEAPI::isLinux))
        .Register();
}
