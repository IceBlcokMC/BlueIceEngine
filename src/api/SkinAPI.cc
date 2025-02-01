#include "api/SkinAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"
#include "endstone/skin.h"
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


Local<Value> SkinAPI::toString(Arguments const& /* args */) { return ConvertToScript("<Skin>"); }
Local<Value> SkinImageDataAPI::toString(Arguments const& /* args */) { return ConvertToScript("<SkinImageData>"); }


/* SkinAPI */
Local<Value> SkinAPI::getSkinId(Arguments const& /* args */) { return ConvertToScript(get()->getSkinId()); }

Local<Value> SkinAPI::getSkinData(Arguments const& /* args */) {
    try {
        return SkinImageDataAPI::newInstance(get()->getSkinData());
    }
    Catch;
}

Local<Value> SkinAPI::getCapeId(Arguments const& /* args */) {
    try {
        auto capeId = get()->getCapeId();
        if (capeId.has_value()) {
            return ConvertToScript(capeId.value());
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> SkinAPI::getCapeData(Arguments const& /* args */) {
    try {
        auto capeData = get()->getCapeData();
        if (capeData.has_value()) {
            return SkinImageDataAPI::newInstance(capeData.value());
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
                ConvertToCpp<int>(args[0]),
                ConvertToCpp<int>(args[1]),
                ConvertToCpp<string>(args[2])
            }
        );
    }
    return nullptr;
}

Local<Value> SkinImageDataAPI::GetterHeight() { return ConvertToScript(this->mSkinImageData.height); }
Local<Value> SkinImageDataAPI::GetterWidth() { return ConvertToScript(this->mSkinImageData.width); }
Local<Value> SkinImageDataAPI::GetterData() { return ConvertToScript(this->mSkinImageData.data); }

void SkinImageDataAPI::SetterHeight(Local<Value> const& value) {
    this->mSkinImageData.height = ConvertToCpp<int>(value);
}
void SkinImageDataAPI::SetterWidth(Local<Value> const& value) { this->mSkinImageData.width = ConvertToCpp<int>(value); }
void SkinImageDataAPI::SetterData(Local<Value> const& value) {
    this->mSkinImageData.data = ConvertToCpp<string>(value);
}


} // namespace jse