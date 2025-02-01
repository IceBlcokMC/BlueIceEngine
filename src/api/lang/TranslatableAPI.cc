#include "api/lang/TranslatableAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"
#include "endstone/lang/translatable.h"
#include "utils/Using.h"


namespace jse {

ClassDefine<TranslatableAPI> TranslatableAPI::builder =
    defineClass<TranslatableAPI>("Translatable")
        .constructor(&TranslatableAPI::make)
        .instanceFunction("toString", &TranslatableAPI::toString)
        .instanceFunction("getText", &TranslatableAPI::getText)
        .instanceFunction("getParameters", &TranslatableAPI::getParameters)
        .instanceFunction("empty", &TranslatableAPI::empty)
        .build();

TranslatableAPI* TranslatableAPI::make(Arguments const& args) {
    try {
        if (args.size() == 2 && args[0].isString() && args[1].isArray()) {
            return new TranslatableAPI(
                args.thiz(),
                endstone::Translatable{ConvertToCpp<string>(args[0]), ConvertToCpp<std::vector<string>>(args[1])}
            );
        }
        return nullptr;
    }
    CatchNotReturn;
    return nullptr;
}


Local<Value> TranslatableAPI::toString(Arguments const& /* args */) { return ConvertToScript("<Translatable>"); }

Local<Value> TranslatableAPI::getText(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getText());
    }
    Catch;
}

Local<Value> TranslatableAPI::getParameters(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getParameters());
    }
    Catch;
}

Local<Value> TranslatableAPI::empty(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().empty());
    }
    Catch;
}


} // namespace jse