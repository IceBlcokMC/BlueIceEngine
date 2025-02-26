#pragma once
#include "V8Scope.h"
#include "boost/pfr.hpp"
#include "boost/pfr/core.hpp"
#include "boost/pfr/core_name.hpp"
#include "v8-context.h"
#include "v8-exception.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-primitive.h"
#include "v8-value.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <type_traits>
#include <unordered_map>
#include <vector>


namespace jse {


namespace detail {

//-----------------------------------------------
// Converter
//-----------------------------------------------
template <typename T, typename Enable = void>
struct Converter;


//-----------------------------------------------
// Basic types
//-----------------------------------------------
template <size_t N>
struct Converter<char[N]> {
    static std::string toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsString()) {
            v8::String::Utf8Value utf8(ctx->GetIsolate(), value);
            return *utf8;
        }
        return {};
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, const char* value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::String::NewFromUtf8(
                                 ctx->GetIsolate(),
                                 value,
                                 v8::NewStringType::kNormal,
                                 static_cast<int>(strlen(value))
        )
                                 .ToLocalChecked());
    }
};

template <size_t N>
struct Converter<const char[N]> {
    static std::string toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsString()) {
            v8::String::Utf8Value utf8(ctx->GetIsolate(), value);
            return *utf8;
        }
        return {};
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, const char* value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::String::NewFromUtf8(
                                 ctx->GetIsolate(),
                                 value,
                                 v8::NewStringType::kNormal,
                                 static_cast<int>(strlen(value))
        )
                                 .ToLocalChecked());
    }
};

template <>
struct Converter<const char*> {
    static std::string toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsString()) {
            v8::String::Utf8Value utf8(ctx->GetIsolate(), value);
            return *utf8;
        }
        return {};
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, const char* value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::String::NewFromUtf8(
                                 ctx->GetIsolate(),
                                 value,
                                 v8::NewStringType::kNormal,
                                 static_cast<int>(strlen(value))
        )
                                 .ToLocalChecked());
    }
};

template <>
struct Converter<std::string> {
    static std::string toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsString()) {
            v8::String::Utf8Value utf8(ctx->GetIsolate(), value);
            return *utf8;
        }
        return {};
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, const std::string& value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::String::NewFromUtf8(
                                 ctx->GetIsolate(),
                                 value.c_str(),
                                 v8::NewStringType::kNormal,
                                 static_cast<int>(value.size())
        )
                                 .ToLocalChecked());
    }
};

template <std::integral T>
struct Converter<T> {
    static T toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsNumber()) {
            return static_cast<T>(value->NumberValue(ctx));
        }
        return 0;
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, T value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::Number::New(ctx->GetIsolate(), static_cast<double>(value)));
    }
};

template <std::floating_point T>
struct Converter<T> {
    static T toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsNumber()) {
            return static_cast<T>(value->NumberValue(ctx));
        }
        return 0;
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, T value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::Number::New(ctx->GetIsolate(), static_cast<double>(value)));
    }
};

template <>
struct Converter<bool> {
    static bool toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsBoolean()) {
            return value->BooleanValue(ctx->GetIsolate());
        }
        return false;
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, bool value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::Boolean::New(ctx->GetIsolate(), value));
    }
};

template <typename T>
struct Converter<T, std::enable_if_t<std::is_enum_v<T>>> {
    static T toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> value) {
        if (value->IsNumber()) {
            return static_cast<T>(value->NumberValue(ctx));
        }
        throw std::runtime_error("Invalid enum value");
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, T const& value) {
        HandleV8Scope hscope(ctx->GetIsolate());
        return hscope.Escape(v8::Number::New(ctx->GetIsolate(), static_cast<int>(value)));
    }
};


//-----------------------------------------------
// Pfr Converter
//-----------------------------------------------
template <typename T>
constexpr bool IsReflectable = std::is_class_v<T> &&                  // 只处理类
                               std::is_aggregate_v<T> &&              // 可聚合初始化
                               !std::is_array_v<T> &&                 // 不是数组
                               !requires(T& a) { a.operator[]; } &&   // 没有重载 operator[]
                               !requires { typename T::value_type; }; // 不是容器类型

template <typename T>
struct Converter<T, std::enable_if_t<IsReflectable<T>>> {
    static T toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val, T res = {}) {
        auto obj = val->ToObject(ctx).ToLocalChecked();
        boost::pfr::for_each_field(res, [&](auto& field, std::size_t index) {
            using FieldType = std::remove_cvref_t<decltype(field)>;
            auto isolate    = ctx->GetIsolate();
            field           = Converter<FieldType>::toCpp(
                isolate,
                ctx,
                obj->Get(ctx, v8::String::NewFromUtf8(isolate, boost::pfr::names_as_array<T>()[index]).ToLocalChecked())
                    .ToLocalChecked()
            );
        });
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, T const& value) {
        auto          isolate = ctx->GetIsolate();
        HandleV8Scope hscope(isolate);

        auto obj = v8::Object::New(isolate);
        boost::pfr::for_each_field(value, [&](auto const& field, std::size_t index) {
            using FieldType = std::remove_cvref_t<decltype(field)>;
            obj->Set(
                ctx,
                v8::String::NewFromUtf8(isolate, boost::pfr::names_as_array<T>()[index]).ToLocalChecked(),
                Converter<FieldType>::toV8(isolate, ctx, field)
            );
        });
        return hscope.Escape(obj);
    }
};


//-----------------------------------------------
// Stl Converter
//-----------------------------------------------
template <typename T>
struct Converter<std::vector<T>> {
    static std::vector<T> toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val) {
        std::vector<T> res;
        if (val->IsArray()) {
            auto arr = val.As<v8::Array>();
            for (uint32_t i = 0; i < arr->Length(); ++i) {
                res.push_back(ConvertToCpp<T>(ctx->GetIsolate(), ctx, arr->Get(ctx, i).ToLocalChecked()));
            }
        }
        return res;
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, std::vector<T> const& value) {
        auto          isolate = ctx->GetIsolate();
        HandleV8Scope hscope(isolate);
        auto          arr = v8::Array::New(isolate, value.size());
        for (uint32_t i = 0; i < value.size(); ++i) {
            arr->Set(ctx, i, ConvertToV8(isolate, ctx, value[i]));
        }
    }
};


template <typename V>
struct Converter<std::unordered_map<std::string, V>> {
    static std::unordered_map<std::string, V> toCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val) {
        std::unordered_map<std::string, V> res;
        if (val->IsObject()) {
            v8::Local<v8::Object> obj           = val.As<v8::Object>();
            v8::Local<v8::Array>  propertyNames = obj->GetPropertyNames(ctx).ToLocalChecked();

            auto isolate = ctx->GetIsolate();
            for (uint32_t i = 0; i < propertyNames->Length(); ++i) {
                v8::Local<v8::Value>  key = propertyNames->Get(ctx, i).ToLocalChecked();
                v8::String::Utf8Value utf8Key(isolate, key);

                std::string keyStr(*utf8Key);

                auto value = obj->Get(ctx, key).ToLocalChecked();

                res[keyStr] = ConvertToCpp<V>(isolate, ctx, value);
            }
        }
        return res;
    }
    static v8::Local<v8::Value> toV8(v8::Local<v8::Context> ctx, std::unordered_map<std::string, V> const& value) {
        auto isolate = ctx->GetIsolate();

        HandleV8Scope hscope(isolate);
        auto          obj = v8::Object::New(isolate);
        for (auto const& [key, val] : value) {
            obj->Set(
                ctx,
                v8::String::NewFromUtf8(isolate, key.c_str()).ToLocalChecked(),
                ConvertToV8(isolate, ctx, val)
            );
        }
        return hscope.Escape(obj);
    }
};


} // namespace detail


template <typename T>
[[nodiscard]] inline T ConvertToCpp(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val) {
    return detail::Converter<T>::toCpp(ctx, val);
}

template <typename T>
[[nodiscard]] inline v8::Local<v8::Value> ConvertToV8(v8::Local<v8::Context> ctx, T const& value) {
    return detail::Converter<T>::toV8(ctx, value);
}


} // namespace jse