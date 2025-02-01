#pragma once
#include "detail/Converter.hpp"

#include "detail/StlConverter.hpp"

#include "detail/ProjectConverters.hpp"


namespace jse {


template <typename T>
[[nodiscard]] Local<Value> ConvertToScript(T const& value) {
    if constexpr (std::is_enum_v<T>) {
        return detail::Converter<T>::toScript(value);
    } else {
        return detail::Converter<std::remove_cvref_t<T>>::toScript(value);
    }
}

template <typename T>
[[nodiscard]] T ConvertToCpp(Local<Value> const& value) {
    if constexpr (std::is_enum_v<T>) {
        return detail::Converter<T>::toCpp(value);
    } else {
        return detail::Converter<std::remove_cvref_t<T>>::toCpp(value);
    }
}

template <typename T>
[[nodiscard]] std::vector<T> ConvertToCpp(Arguments const& args) {
    std::vector<T> result;
    result.reserve(args.size());
    for (size_t i = 0; i < args.size(); i++) {
        result.push_back(ConvertToCpp<T>(args[i]));
    }
    return result;
}


} // namespace jse