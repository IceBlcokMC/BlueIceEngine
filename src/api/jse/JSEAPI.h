#pragma once
#include "ScriptBackend.hpp"
#include "utils/Using.h"
#include "v8-exception.h"
#include "v8-initialization.h"

namespace jse {

class JSEAPI {
public:
    bool registerPlugin() {return false;}

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

} // namespace jse

UsingCppType(jse::JSEAPI);

void RegisterJSEAPI() {
    puerts::DefineClass<jse::JSEAPI>()
        .Function("registerPlugin", MakeCheckFunction(&jse::JSEAPI::registerPlugin))
        .Function("isWindows", MakeCheckFunction(&jse::JSEAPI::isWindows))
        .Function("isLinux", MakeCheckFunction(&jse::JSEAPI::isLinux))
        .Register();
}