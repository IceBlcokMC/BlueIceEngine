#pragma once
#include "api/lang/TranslatableAPI.h"
#include "detail/Converter.hpp"


namespace jse::detail {


template <>
struct Converter<endstone::Translatable> {
    static Local<Value> toScript(endstone::Translatable const& tr) { return TranslatableAPI::newInstance(tr); }
    static Local<Value> toScript(endstone::Translatable& tr) { return TranslatableAPI::newInstance(tr); }

    static endstone::Translatable& toCpp(Local<Value> const& val) {
        if (EngineScope::currentEngine()->isInstanceOf<TranslatableAPI>(val)) {
            return GetScriptClass(TranslatableAPI, val)->get();
        }
        throw script::Exception("Failed to convert value to Translatable");
    }
};

template <>
struct Converter<TranslatableAPI> {
    static Local<Value> toScript(TranslatableAPI const& tr) { return tr.getScriptObject(); }
    static Local<Value> toScript(TranslatableAPI& tr) { return tr.getScriptObject(); }

    static TranslatableAPI* toCpp(Local<Value> const& val) {
        if (EngineScope::currentEngine()->isInstanceOf<TranslatableAPI>(val)) {
            return GetScriptClass(TranslatableAPI, val);
        }
        throw script::Exception("Failed to convert value to Translatable");
    }
};


} // namespace jse::detail