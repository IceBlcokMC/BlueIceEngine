#pragma once
#include "ScriptBackend.hpp"
#include "endstone/permissions/permission_default.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_description.h"
#include "endstone/plugin/plugin_load_order.h"
#include "puerts_impl/ConverterImpl.h"
#include "puerts_impl/EnumImpl.h"
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


struct TestBuilder {
    std::string name{"unnamed"};
    std::string version;
    std::string description;

    std::function<void()> onLoad;
    std::function<void()> onEnable;
    std::function<void()> onDisable;
};

class JSEAPI {
public:
    bool registerPlugin(TestBuilder* builder) { return false; }

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

UsingCppType(TestBuilder);
UsingCppType(JSEAPI);

struct AutoRegisterJSEAPI {
    AutoRegisterJSEAPI() {
        puerts::DefineClass<TestBuilder>()
            .Constructor()
            .Property("name", MakeProperty(&TestBuilder::name))
            .Property("version", MakeProperty(&TestBuilder::version))
            .Property("description", MakeProperty(&TestBuilder::description))
            .Property("onLoad", MakeVariable(&TestBuilder::onLoad))
            .Property("onEnable", MakeVariable(&TestBuilder::onEnable))
            .Property("onDisable", MakeVariable(&TestBuilder::onDisable))
            .Register();

        puerts::DefineClass<JSEAPI>()
            .Function("registerPlugin", MakeFunction(&JSEAPI::registerPlugin))
            // .Function("getSelf", MakeFunction(&JSEAPI::getSelf))
            .Function("isWindows", MakeFunction(&JSEAPI::isWindows))
            .Function("isLinux", MakeFunction(&JSEAPI::isLinux))
            .Register();
    }
};
AutoRegisterJSEAPI __AutoRegisterJSEAPI__;
