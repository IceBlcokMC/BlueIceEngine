#pragma once
#include "endstone/skin.h"
#include "utils/Defines.h"
#include "utils/Using.h"

namespace jse {


class SkinAPI : public ScriptClass {
    endstone::Skin* mSkin;

public:
    explicit SkinAPI(endstone::Skin* skin) : ScriptClass(ScriptClass::ConstructFromCpp<SkinAPI>{}), mSkin(skin) {};

    static Local<Object> newInstance(endstone::Skin* skin) { return (new SkinAPI(skin))->getScriptObject(); }

    endstone::Skin* get() { return mSkin; }

public: /* Method */
    METHODS(toString);
    METHODS(getSkinId);
    METHODS(getSkinData);
    METHODS(getCapeId);
    METHODS(getCapeData);

public:
    static ClassDefine<SkinAPI> builder;
};


class SkinImageDataAPI : public ScriptClass {
    endstone::Skin::ImageData mSkinImageData;

public:
    // C++ new
    explicit SkinImageDataAPI(endstone::Skin::ImageData data)
    : ScriptClass(ConstructFromCpp<SkinImageDataAPI>{}),
      mSkinImageData(std::move(data)) {}

    static Local<Object> newInstance(endstone::Skin::ImageData data) {
        return (new SkinImageDataAPI(std::move(data)))->getScriptObject();
    }

    // Js new
    explicit SkinImageDataAPI(Local<Object> const& thiz, endstone::Skin::ImageData data)
    : ScriptClass(thiz),
      mSkinImageData(std::move(data)) {}

    static SkinImageDataAPI* make(Arguments const& args);

    endstone::Skin::ImageData& get() { return mSkinImageData; }

public: /* instanceProperty */
    Local<Value> GetterHeight();
    Local<Value> GetterWidth();
    Local<Value> GetterData();

    void SetterHeight(Local<Value> const& value);
    void SetterWidth(Local<Value> const& value);
    void SetterData(Local<Value> const& value);

public: /* Method */
    METHODS(toString);

public:
    static ClassDefine<SkinImageDataAPI> builder;
};


} // namespace jse