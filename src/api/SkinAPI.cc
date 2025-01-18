#include "api/SkinAPI.h"
#include "api/APIHelper.h"
#include "endstone/skin.h"
#include "utils/Convert.h"
#include "utils/Using.h"


namespace jse {

ClassDefine<SkinAPI> SkinAPI::builder = defineClass<SkinAPI>("Skin")
                                            .constructor(nullptr)
                                            .instanceFunction("getSkinId", &SkinAPI::getSkinId)
                                            .instanceFunction("getSkinData", &SkinAPI::getSkinData)
                                            .instanceFunction("getCapeId", &SkinAPI::getCapeId)
                                            .instanceFunction("getCapeData", &SkinAPI::getCapeData)
                                            .build();

ClassDefine<SkinImageDataAPI> SkinImageDataAPI::builder =
    defineClass<SkinImageDataAPI>("SkinImageData")
        .constructor(&SkinImageDataAPI::make)
        .instanceProperty("height", &SkinImageDataAPI::GetterHeight, &SkinImageDataAPI::SetterHeight)
        .instanceProperty("width", &SkinImageDataAPI::GetterWidth, &SkinImageDataAPI::SetterWidth)
        .instanceProperty("data", &SkinImageDataAPI::GetterData, &SkinImageDataAPI::SetterData)
        .build();


Local<Value> SkinAPI::toString(Arguments const& /* args */) { return ConvertToScriptX("<Skin>"); }
Local<Value> SkinImageDataAPI::toString(Arguments const& /* args */) { return ConvertToScriptX("<SkinImageData>"); }


/* SkinAPI */
Local<Value> SkinAPI::getSkinId(Arguments const& /* args */) { return ConvertToScriptX(get()->getSkinId()); }

Local<Value> SkinAPI::getSkinData(Arguments const& /* args */) {
    try {
        return SkinImageDataAPI::newSkinImageDataAPI(get()->getSkinData());
    }
    Catch;
}

Local<Value> SkinAPI::getCapeId(Arguments const& /* args */) {
    try {
        auto capeId = get()->getCapeId();
        if (capeId.has_value()) {
            return ConvertToScriptX(capeId.value());
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> SkinAPI::getCapeData(Arguments const& /* args */) {
    try {
        auto capeData = get()->getCapeData();
        if (capeData.has_value()) {
            return SkinImageDataAPI::newSkinImageDataAPI(capeData.value());
        }
        return Local<Value>();
    }
    Catch;
}


/* SkinImageDataAPI */
SkinImageDataAPI* SkinImageDataAPI::make(Arguments const& args) {
    if (args.size() == 3 && args[0].isNumber() && args[1].isNumber() && args[2].isString()) {
        return new SkinImageDataAPI(
            args.thiz(),
            endstone::Skin::ImageData{
                ConvertFromScriptX<int>(args[0]),
                ConvertFromScriptX<int>(args[1]),
                ConvertFromScriptX<string>(args[2])
            }
        );
    }
    return nullptr;
}

Local<Value> SkinImageDataAPI::GetterHeight() { return ConvertToScriptX(this->mSkinImageData.height); }
Local<Value> SkinImageDataAPI::GetterWidth() { return ConvertToScriptX(this->mSkinImageData.width); }
Local<Value> SkinImageDataAPI::GetterData() { return ConvertToScriptX(this->mSkinImageData.data); }

void SkinImageDataAPI::SetterHeight(Local<Value> const& value) {
    this->mSkinImageData.height = ConvertFromScriptX<int>(value);
}
void SkinImageDataAPI::SetterWidth(Local<Value> const& value) {
    this->mSkinImageData.width = ConvertFromScriptX<int>(value);
}
void SkinImageDataAPI::SetterData(Local<Value> const& value) {
    this->mSkinImageData.data = ConvertFromScriptX<string>(value);
}


} // namespace jse