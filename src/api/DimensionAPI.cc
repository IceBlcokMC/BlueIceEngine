#include "DimensionAPI.h"
#include "utils/Convert.h"

namespace jse {
ClassDefine<DimensionAPI> DimensionAPI::builder = defineClass<DimensionAPI>("Dimension")
                                                      .constructor(nullptr)
                                                      .instanceFunction("toString", &DimensionAPI::toString)
                                                      .instanceFunction("getName", &DimensionAPI::getName)
                                                      .instanceFunction("getType", &DimensionAPI::getType)
                                                      .instanceFunction("getLevel", &DimensionAPI::getLevel)
                                                      .instanceFunction("getBlockAt", &DimensionAPI::getBlockAt)
                                                      .build();
Local<Value> DimensionAPI::toString(Arguments const& /*value*/) { return ConvertToScriptX("<Dimension>"); }
Local<Value> DimensionAPI::getName(Arguments const& /*value*/) { return ConvertToScriptX(get()->getName()); }
Local<Value> DimensionAPI::getType(Arguments const& /*value*/) { return ConvertToScriptX(get()->getType()); }
Local<Value> DimensionAPI::getLevel(Arguments const& /*value*/) { return Local<Value>(); }
Local<Value> DimensionAPI::getBlockAt(Arguments const& /*value*/) { return Local<Value>(); }

} // namespace jse