#pragma once
#include "api/APIHelper.h"
#include "endstone/form/controls/dropdown.h"
#include "endstone/message.h"
#include "utils/Using.h"
#include <algorithm>
#include <optional>
#include <vector>

namespace jse {


class DropdownAPI : public ScriptClass {
    endstone::Dropdown mDropdown;

public:
    explicit DropdownAPI(
        Local<Object> const& thiz,
        endstone::Message    label,
        std::vector<string>  options,
        std::optional<int>   defaultIndex = std::nullopt
    )
    : ScriptClass(thiz),
      mDropdown(std::move(label), std::move(options), defaultIndex) {}

    endstone::Dropdown& get() { return mDropdown; }

    static DropdownAPI* make(Arguments const& args);

public:
    METHODS(toString);
    METHODS(getLabel);
    METHODS(setLabel);
    METHODS(addOption);
    METHODS(getOptions);
    METHODS(setOptions);
    METHODS(getDefaultIndex);
    METHODS(setDefaultIndex);

public:
    static ClassDefine<DropdownAPI> builder;
};


} // namespace jse