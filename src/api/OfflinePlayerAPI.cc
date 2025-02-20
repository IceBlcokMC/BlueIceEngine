#include "OfflinePlayerAPI.h"
#include "converter/Convert.h"
#include "utils/Using.h"


namespace jse {

ClassDefine<OfflinePlayerAPI> OfflinePlayerAPI::builder =
    defineClass<OfflinePlayerAPI>("OfflinePlayer")
        .instanceFunction("getName", &OfflinePlayerAPI::getName)
        .instanceFunction("getUniqueId", &OfflinePlayerAPI::getUniqueId)
        .build();


Local<Value> OfflinePlayerAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScript("OfflinePlayer");
    }
    Catch;
}

Local<Value> OfflinePlayerAPI::getName(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getName());
    }
    Catch;
}

Local<Value> OfflinePlayerAPI::getUniqueId(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getUniqueId());
    }
    Catch;
}


} // namespace jse