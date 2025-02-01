#include "DropdownAPI.h"
#include "api/APIHelper.h"
#include "api/form/FormAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "converter/Convert.h"
#include "endstone/message.h"
#include "utils/Using.h"


namespace jse {

ClassDefine<DropdownAPI> DropdownAPI::builder = defineClass<DropdownAPI>("Dropdown")
                                                    .constructor(&DropdownAPI::make)
                                                    .instanceFunction("toString", &DropdownAPI::toString)
                                                    .instanceFunction("getLabel", &DropdownAPI::getLabel)
                                                    .instanceFunction("setLabel", &DropdownAPI::setLabel)
                                                    .instanceFunction("addOption", &DropdownAPI::addOption)
                                                    .instanceFunction("getOptions", &DropdownAPI::getOptions)
                                                    .instanceFunction("setOptions", &DropdownAPI::setOptions)
                                                    .instanceFunction("getDefaultIndex", &DropdownAPI::getDefaultIndex)
                                                    .instanceFunction("setDefaultIndex", &DropdownAPI::setDefaultIndex)
                                                    .build();


DropdownAPI* DropdownAPI::make(Arguments const& args) {
    if (args.size() < 2) {
        return nullptr;
    }
    if (!args[2].isArray()) {
        return nullptr;
    }

    std::optional<int> defaultIndex = std::nullopt;
    if (args.size() == 3) {
        defaultIndex = ConvertToCpp<int>(args[2]);
    }

    endstone::Message label;
    if (args[0].isString()) {
        label = ConvertToCpp<string>(args[0]);
    } else if (args[0].isObject() && IsInstanceOf<TranslatableAPI>(args[0])) {
        label = GetScriptClass(TranslatableAPI, args[0])->get();
    } else {
        return nullptr;
    }

    return new DropdownAPI(args.thiz(), label, ConvertToCpp<std::vector<string>>(args[1]), defaultIndex);
}


Local<Value> DropdownAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScript("<Dropdown>");
    }
    Catch;
}

Local<Value> DropdownAPI::getLabel(Arguments const& /* args */) {
    try {
        return detail::ConvertVariantToScriptX(get().getLabel());
    }
    Catch;
}

Local<Value> DropdownAPI::setLabel(Arguments const& args) {
    try {
    }
    Catch;
}

Local<Value> DropdownAPI::addOption(Arguments const& args) {
    try {
    }
    Catch;
}

Local<Value> DropdownAPI::getOptions(Arguments const& args) {
    try {
    }
    Catch;
}

Local<Value> DropdownAPI::setOptions(Arguments const& args) {
    try {
    }
    Catch;
}

Local<Value> DropdownAPI::getDefaultIndex(Arguments const& args) {
    try {
    }
    Catch;
}

Local<Value> DropdownAPI::setDefaultIndex(Arguments const& args) {
    try {
    }
    Catch;
}


} // namespace jse