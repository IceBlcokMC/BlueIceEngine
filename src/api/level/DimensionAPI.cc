#include "DimensionAPI.h"
#include "LevelAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"


namespace jse {


ClassDefine<DimensionAPI> DimensionAPI::builder =
    defineClass<DimensionAPI>("Dimension")
        .constructor(nullptr)
        .instanceFunction("toString", &DimensionAPI::toString)
        .instanceFunction("getName", &DimensionAPI::getName)
        .instanceFunction("getType", &DimensionAPI::getType)
        .instanceFunction("getLevel", &DimensionAPI::getLevel)
        .instanceFunction("getBlockAt", &DimensionAPI::getBlockAt)
        .instanceFunction("getHighestBlockYAt", &DimensionAPI::getHighestBlockYAt)
        .instanceFunction("getHighestBlockAt", &DimensionAPI::getHighestBlockAt)
        .instanceFunction("getLoadedChunks", &DimensionAPI::getLoadedChunks)
        .build();


Local<Value> DimensionAPI::toString(Arguments const& /* args */) { return ConvertToScript("<Dimension>"); }

Local<Value> DimensionAPI::getName(Arguments const& /* args */) { return ConvertToScript(get()->getName()); }

Local<Value> DimensionAPI::getType(Arguments const& /* args */) { return ConvertToScript(get()->getType()); }

Local<Value> DimensionAPI::getLevel(Arguments const& /* args */) { return LevelAPI::newInstance(&get()->getLevel()); }

Local<Value> DimensionAPI::getBlockAt(Arguments const& /* args */) { return Local<Value>(); } // TODO: BlockAPI

Local<Value> DimensionAPI::getHighestBlockYAt(Arguments const& args) {
    try {
        CheckArgsCount(args, 2);
        CheckArgType(args[0], ValueKind::kNumber);
        CheckArgType(args[1], ValueKind::kNumber);
        return ConvertToScript(get()->getHighestBlockYAt(ConvertToCpp<int>(args[0]), ConvertToCpp<int>(args[1])));
    }
    Catch;
}

Local<Value> DimensionAPI::getHighestBlockAt(Arguments const& /* args */) {
    try {
        return {}; // TODO: BlockAPI
    }
    Catch;
}

Local<Value> DimensionAPI::getLoadedChunks(Arguments const& /* args */) {
    try {
        return {}; // TODO: ChunkAPI
    }
    Catch;
}

} // namespace jse