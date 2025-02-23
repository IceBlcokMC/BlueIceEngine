#pragma once
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-value.h"
#include "v8_utils/Converter.h"
#include "v8_utils/V8Exception.h"

namespace jse {

class V8ObjectProxy {
    v8::Isolate*           isolate_;
    v8::Local<v8::Context> context_;
    v8::Local<v8::Value>   value_;

public:
    V8ObjectProxy(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> value)
    : isolate_(isolate),
      context_(context),
      value_(value) {}

    V8ObjectProxy operator[](const std::string& key) const {
        if (!value_->IsObject()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Value is not an object").ToLocalChecked())
            );
        }

        auto obj    = value_.As<v8::Object>();
        auto result = obj->Get(context_, ConvertToV8(isolate_, context_, key));
        if (result.IsEmpty()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Failed to get property").ToLocalChecked())
            );
        }
        return {isolate_, context_, result.ToLocalChecked()};
    }

    V8ObjectProxy operator[](int index) const { return operator[](std::to_string(index)); }

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
    [[nodiscard]] T As() const {
        return ConvertToCpp<T>(isolate_, context_, value_);
    }

    [[nodiscard]] v8::Local<v8::Value> GetValue() const { return value_; }
};

class V8Object {
    v8::Isolate*           isolate_;
    v8::Local<v8::Context> context_;
    v8::Local<v8::Object>  object_;

public:
    V8Object(v8::Isolate* isolate, v8::Local<v8::Context> context)
    : isolate_(isolate),
      context_(context),
      object_(v8::Object::New(isolate)) {}

    V8Object(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> object)
    : isolate_(isolate),
      context_(context),
      object_(object) {}

    V8ObjectProxy operator[](const std::string& key) {
        return {isolate_, context_, object_->Get(context_, ConvertToV8(isolate_, context_, key)).ToLocalChecked()};
    }

    V8ObjectProxy operator[](int index) { return operator[](std::to_string(index)); }

    template <typename T>
    bool Set(const std::string& key, const T& value) {
        return !object_->Set(context_, ConvertToV8(isolate_, context_, key), ConvertToV8(isolate_, context_, value))
                    .IsNothing();
    }

    template <typename T>
    T Get(const std::string& key) {
        auto value = object_->Get(context_, ConvertToV8(isolate_, context_, key));
        if (value.IsEmpty()) {
            throw v8_exception(
                isolate_,
                context_,
                v8::Exception::Error(v8::String::NewFromUtf8(isolate_, "Failed to get property").ToLocalChecked())
            );
        }
        return ConvertToCpp<T>(isolate_, context_, value.ToLocalChecked());
    }

    [[nodiscard]] v8::Local<v8::Object> GetObject() const { return object_; }
};

} // namespace jse