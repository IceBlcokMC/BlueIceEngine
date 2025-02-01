#include "DimensionAPI.h"
#include "LevelAPI.h"
#include "converter/Convert.h"


namespace jse {


ClassDefine<DimensionAPI> DimensionAPI::builder = defineClass<DimensionAPI>("Dimension")
                                                      .constructor(nullptr)
                                                      .instanceFunction("toString", &DimensionAPI::toString)
                                                      .instanceFunction("getName", &DimensionAPI::getName)
                                                      .instanceFunction("getType", &DimensionAPI::getType)
                                                      .instanceFunction("getLevel", &DimensionAPI::getLevel)
                                                      .instanceFunction("getBlockAt", &DimensionAPI::getBlockAt)
                                                      .build();


Local<Value> DimensionAPI::toString(Arguments const& /* args */) { return ConvertToScript("<Dimension>"); }

Local<Value> DimensionAPI::getName(Arguments const& /* args */) { return ConvertToScript(get()->getName()); }

Local<Value> DimensionAPI::getType(Arguments const& /* args */) { return ConvertToScript(get()->getType()); }

Local<Value> DimensionAPI::getLevel(Arguments const& /* args */) { return LevelAPI::newInstance(&get()->getLevel()); }

Local<Value> DimensionAPI::getBlockAt(Arguments const& /* args */) { return Local<Value>(); } // TODO: BlockAPI

} // namespace jse