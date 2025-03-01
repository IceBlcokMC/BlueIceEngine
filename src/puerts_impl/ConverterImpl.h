#pragma once
#include "ScriptBackend.hpp"
#include "v8-container.h"
#include <unordered_set>


namespace puerts {

namespace v8_impl {

template <typename T>
struct Converter<std::vector<T>> {
    static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, std::vector<T> const& value) {
        auto ret = v8::Array::New(context->GetIsolate(), value.size());
        for (size_t i = 0; i < value.size(); i++) {
            (ret->Set(context, static_cast<int>(i), Converter<T>::toScript(context, value[i]))).Check();
        }
        return ret;
    }

    static std::vector<T> toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value) {
        std::vector<T> ret;
        if (value->IsArray()) {
            ret.reserve(value.As<v8::Array>()->Length());
            auto arr = value.As<v8::Array>();
            for (size_t i = 0; i < arr->Length(); i++) {
                ret.push_back(Converter<T>::toCpp(context, arr->Get(context, static_cast<int>(i)).ToLocalChecked()));
            }
        }
        return ret;
    }

    static bool accept([[maybe_unused]] v8::Local<v8::Context> context, const v8::Local<v8::Value>& value) {
        return value->IsArray();
    }
};
} // namespace v8_impl

template <typename T>
struct ScriptTypeName<std::vector<T>> {
    static constexpr auto value() { return ScriptTypeName<T>::value() + internal::Literal("[]"); }
};


// std::unordered_set
namespace v8_impl {

template <typename T>
struct Converter<std::unordered_set<T>> {
    static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, std::unordered_set<T> const& value) {
        auto ret = v8::Array::New(context->GetIsolate(), value.size());
        int  i   = 0;
        for (const auto& item : value) {
            (ret->Set(context, static_cast<int>(i++), Converter<T>::toScript(context, item))).Check();
        }
        return ret;
    }
    static std::unordered_set<T> toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value) {
        std::unordered_set<T> ret;
        if (value->IsArray()) {
            ret.reserve(value.As<v8::Array>()->Length());
            auto arr = value.As<v8::Array>();
            for (size_t i = 0; i < arr->Length(); i++) {
                ret.insert(Converter<T>::toCpp(context, arr->Get(context, static_cast<int>(i)).ToLocalChecked()));
            }
        }
        return ret;
    }
    static bool accept([[maybe_unused]] v8::Local<v8::Context> context, const v8::Local<v8::Value>& value) {
        return value->IsArray();
    }
};

} // namespace v8_impl

template <typename T>
struct ScriptTypeName<std::unordered_set<T>> {
    static constexpr auto value() { return ScriptTypeName<T>::value() + internal::Literal("[]"); }
};

} // namespace puerts
