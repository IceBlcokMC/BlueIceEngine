#include "api/level/PositionAPI.h"
#include "DimensionAPI.h"
#include "api/APIHelper.h"
#include "api/actor/ActorAPI.h"
#include "converter/Convert.h"



namespace jse {

ClassDefine<PositionAPI> PositionAPI::builder = defineClass<PositionAPI>("Position")
                                                    .constructor(nullptr)
                                                    .instanceFunction("toString", &PositionAPI::toString)
                                                    .instanceFunction("getDimension", &PositionAPI::getDimension)
                                                    .instanceFunction("setDimension", &PositionAPI::setDimension)
                                                    .instanceFunction("getBlockX", &PositionAPI::getBlockX)
                                                    .instanceFunction("getBlockY", &PositionAPI::getBlockY)
                                                    .instanceFunction("getBlockZ", &PositionAPI::getBlockZ)

                                                    // PositionAPI extends VectorAPI
                                                    .instanceFunction("getX", &VectorAPI::getX)
                                                    .instanceFunction("getY", &VectorAPI::getY)
                                                    .instanceFunction("getZ", &VectorAPI::getZ)
                                                    .instanceFunction("setX", &VectorAPI::setX)
                                                    .instanceFunction("setY", &VectorAPI::setY)
                                                    .instanceFunction("setZ", &VectorAPI::setZ)
                                                    .instanceFunction("length", &VectorAPI::length)
                                                    .instanceFunction("lengthSquared", &VectorAPI::lengthSquared)
                                                    .instanceFunction("distance", &VectorAPI::distance)
                                                    .instanceFunction("distanceSquared", &VectorAPI::distanceSquared)
                                                    // engine provided
                                                    .instanceProperty("x", &VectorAPI::GetterX, &VectorAPI::SetterX)
                                                    .instanceProperty("y", &VectorAPI::GetterY, &VectorAPI::SetterY)
                                                    .instanceProperty("z", &VectorAPI::GetterZ, &VectorAPI::SetterZ)
                                                    .build();


Local<Value> PositionAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScript("<Position>");
    }
    Catch;
}

Local<Value> PositionAPI::getDimension(Arguments const& /* args */) {
    try {
        auto dim = get()->getDimension();
        if (dim) {
            return DimensionAPI::newInstance(dim);
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> PositionAPI::setDimension(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kObject);
        if (!IsInstanceOf<DimensionAPI>(args[0])) {
            throw script::Exception(ERR_WRONG_ARG_TYPE);
        }
        auto dim = GetScriptClass(DimensionAPI, args[0])->get();
        get()->setDimension(*dim);
        return Local<Value>();
    }
    CatchAndThrow;
}

Local<Value> PositionAPI::getBlockX(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getBlockX());
    }
    Catch;
}

Local<Value> PositionAPI::getBlockY(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getBlockY());
    }
    Catch;
}

Local<Value> PositionAPI::getBlockZ(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getBlockZ());
    }
    Catch;
}


} // namespace jse