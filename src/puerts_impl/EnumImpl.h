#pragma once
#include <map>
#include <string>
#include <string_view>
#include <type_traits>


namespace puerts {

namespace enum_impl {

inline std::map<std::string, std::map<std::string, int>>& getAllEnums() {
    static std::map<std::string, std::map<std::string, int>> instance;
    return instance;
}

template <typename T>
struct JsEnumImpl;

} // namespace enum_impl


template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
[[nodiscard]] inline constexpr std::string_view GetEnumName() {
    return enum_impl::JsEnumImpl<T>::value;
}

} // namespace puerts


#define UsingCppEnum(ENUM)                                                                                             \
    namespace puerts::enum_impl {                                                                                      \
    template <>                                                                                                        \
    struct JsEnumImpl<ENUM> {                                                                                          \
        static_assert(std::is_enum_v<ENUM>);                                                                           \
        constexpr static auto      value = #ENUM;                                                                      \
        std::map<std::string, int> valueMap;                                                                           \
                                                                                                                       \
        JsEnumImpl<ENUM>& Variable(std::string name, ENUM val) {                                                       \
            valueMap[std::move(name)] = static_cast<int>(val);                                                         \
            return *this;                                                                                              \
        }                                                                                                              \
        void Register() { getAllEnums()[value] = std::move(valueMap); }                                                \
    };                                                                                                                 \
    }

#define DefineEnum(ENUM)                                                                                               \
    puerts::enum_impl::JsEnumImpl<ENUM> {}
