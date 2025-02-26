#pragma once
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-value.h"
#include "v8_utils/Converter.h"
#include "v8_utils/V8Exception.h"
#include <vector>

namespace jse {

class V8Object {
    v8::Isolate*           isolate_;
    v8::Local<v8::Context> context_;
    v8::Local<v8::Value>   value_;

public:
    V8Object(v8::Isolate* isolate, v8::Local<v8::Context> context)
    : isolate_(isolate),
      context_(context),
      value_(v8::Object::New(isolate)) {}

    V8Object(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    : isolate_(isolate),
      context_(context),
      value_(value) {}

    V8Object operator[](const std::string& key) const {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto obj    = value_.As<v8::Object>();
        auto result = obj->Get(context_, ConvertToV8(context_, key));
        if (result.IsEmpty()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Failed to get property").ToLocalChecked())
            );
        }
        return {isolate_, context_, result.ToLocalChecked()};
    }

    V8Object operator[](int index) const { return operator[](std::to_string(index)); }

    template <typename T>
    void operator=(const T& value) {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto obj = value_.As<v8::Object>();
        if (obj->Set(context_, ConvertToV8(isolate_, context_, value)).IsNothing()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Failed to set property").ToLocalChecked())
            );
        }
    }

    template <typename T>
    bool Set(const std::string& key, const T& value) {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto obj = value_.As<v8::Object>();
        return !obj->Set(context_, ConvertToV8(context_, key), ConvertToV8(context_, value)).IsNothing();
    }

    template <typename T>
    T Get(const std::string& key) const {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto obj    = value_.As<v8::Object>();
        auto result = obj->Get(context_, ConvertToV8(context_, key));
        if (result.IsEmpty()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Failed to get property").ToLocalChecked())
            );
        }
        return ConvertToCpp<T>(isolate_, context_, result.ToLocalChecked());
    }

    [[nodiscard]] std::vector<std::string> GetAllKeys() const {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto                     obj = value_.As<v8::Object>();
        std::vector<std::string> keys;
        v8::Local<v8::Array>     propertyNames = obj->GetPropertyNames(context_).ToLocalChecked();
        for (uint32_t i = 0; i < propertyNames->Length(); ++i) {
            v8::Local<v8::Value>  key = propertyNames->Get(context_, i).ToLocalChecked();
            v8::String::Utf8Value utf8Key(isolate_, key);
            keys.emplace_back(*utf8Key);
        }
        return keys;
    }

    [[nodiscard]] bool HasKey(const std::string& key) const {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto obj   = value_.As<v8::Object>();
        auto value = obj->Get(context_, ConvertToV8(context_, key));
        return !value.IsEmpty() && !value.ToLocalChecked()->IsUndefined();
    }

    template <typename T>
    T As() const {
        return ConvertToCpp<T>(isolate_, context_, value_);
    }

    [[nodiscard]] v8::Local<v8::Value> GetValue() const { return value_; }

    [[nodiscard]] v8::Local<v8::Object> GetObject() const {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }
        return value_.As<v8::Object>();
    }
};

} // namespace jse