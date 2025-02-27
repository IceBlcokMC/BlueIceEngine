#pragma once


#define UsingNamedCppEnum(ENUM, NAME)                                                                                  \
    namespace PUERTS_NAMESPACE {                                                                                       \
    template <>                                                                                                        \
    struct ScriptTypeName<ENUM> {                                                                                      \
        static constexpr auto value() { return internal ::Literal(#NAME); }                                            \
    };                                                                                                                 \
    }                                                                                                                  \
    /*     namespace PUERTS_NAMESPACE {                                                                                \
        template <>                                                                                                    \
        struct is_objecttype<ENUM> : public std ::true_type {};                                                        \
        }                                                                                                              \
     */                                                                                                                \
    namespace PUERTS_NAMESPACE {                                                                                       \
    namespace v8_impl {                                                                                                \
    template <>                                                                                                        \
    struct Converter<ENUM> {                                                                                           \
        static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, ENUM value) {                             \
            return v8::Number::New(context->GetIsolate(), static_cast<int>(value));                                    \
        }                                                                                                              \
        static ENUM toCpp(v8::Local<v8::Context> context, v8::Local<v8::Value> const& value) {                         \
            return static_cast<ENUM>(value->Int32Value(context).ToChecked());                                          \
        }                                                                                                              \
        static bool accept([[maybe_unused]] v8::Local<v8::Context> context, v8::Local<v8::Value> const& value) {       \
            return value->IsNumber();                                                                                  \
        }                                                                                                              \
    };                                                                                                                 \
    }                                                                                                                  \
    }

#define UsingCppEnum(ENUM) UsingNamedCppEnum(ENUM, ENUM)