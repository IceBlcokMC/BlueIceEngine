#pragma once
#include "api/APIHelper.h"
#include "endstone/lang/translatable.h"
#include "utils/Using.h"



namespace jse {


class TranslatableAPI : public ScriptClass {
    endstone::Translatable mTranslatable;

public:
    // C++ new
    explicit TranslatableAPI(endstone::Translatable translatable)
    : ScriptClass(ConstructFromCpp<TranslatableAPI>{}),
      mTranslatable(std::move(translatable)) {}

    static Local<Object> newInstance(endstone::Translatable translatable) {
        return (new TranslatableAPI(std::move(translatable)))->getScriptObject();
    }

    // Js new
    explicit TranslatableAPI(Local<Object> const& thiz, endstone::Translatable translatable)
    : ScriptClass(thiz),
      mTranslatable(std::move(translatable)) {}

    static TranslatableAPI* make(Arguments const& args);

    endstone::Translatable& get() { return mTranslatable; }

public:
    METHODS(toString);
    METHODS(getText);
    METHODS(getParameters);
    METHODS(empty);

public:
    static ClassDefine<TranslatableAPI> builder;
};


} // namespace jse