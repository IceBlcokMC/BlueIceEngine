#pragma once
#include <map>
#include <string>
#include <string_view>
#include <type_traits>


namespace puerts {

namespace enum_impl {
std::map<std::string, std::map<std::string, int>> AllEnums;
template <typename T>
struct JsEnumImpl {
    static_assert(std::is_enum<T>::value, "JsEnumImpl only support enum type");
};
} // namespace enum_impl


#define UsingCppEnum(ENUM)                                                                                             \
    namespace puerts::enum_impl {                                                                                      \
    template <>                                                                                                        \
    struct JsEnumImpl<ENUM> {                                                                                          \
        constexpr static auto      value = #ENUM;                                                                      \
        std::map<std::string, int> valueMap;                                                                           \
                                                                                                                       \
        inline JsEnumImpl<ENUM>& Variable(std::string name, ENUM val) {                                                \
            valueMap[std::move(name)] = static_cast<int>(val);                                                         \
            return *this;                                                                                              \
        }                                                                                                              \
        inline void Register() { enum_impl::AllEnums[value] = std::move(valueMap); }                                   \
    };                                                                                                                 \
    }

#define DefineEnum(ENUM)                                                                                               \
    puerts::enum_impl::JsEnumImpl<ENUM> {}


template <typename T>
[[nodiscard]] inline constexpr std::string_view GetEnumName() {
    return enum_impl::JsEnumImpl<T>::value;
}


} // namespace puerts
