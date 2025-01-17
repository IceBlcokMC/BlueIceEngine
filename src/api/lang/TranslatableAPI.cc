#include "api/lang/TranslatableAPI.h"
#include "api/APIHelper.h"
#include "endstone/lang/translatable.h"
#include "utils/Convert.h"
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
                endstone::Translatable{
                    ConvertFromScriptX<string>(args[0]),
                    ConvertFromScriptX<std::vector<string>>(args[1])
                }
            );
        }
        return nullptr;
    }
    CatchNotReturn;
    return nullptr;
}


Local<Value> TranslatableAPI::toString(Arguments const& /* args */) { return ConvertToScriptX("<Translatable>"); }

Local<Value> TranslatableAPI::getText(Arguments const& /* args */) {
    try {
        return ConvertToScriptX(get().getText());
    }
    Catch;
}

Local<Value> TranslatableAPI::getParameters(Arguments const& /* args */) {
    try {
        return ConvertToScriptX(get().getParameters());
    }
    Catch;
}

Local<Value> TranslatableAPI::empty(Arguments const& /* args */) {
    try {
        return ConvertToScriptX(get().empty());
    }
    Catch;
}


} // namespace jse