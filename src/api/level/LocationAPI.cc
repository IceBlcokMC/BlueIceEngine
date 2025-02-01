#include "api/level/LocationAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"



namespace jse {

ClassDefine<LocationAPI> LocationAPI::builder = defineClass<LocationAPI>("Location")
                                                    .constructor(nullptr)
                                                    .instanceFunction("toString", &LocationAPI::toString)
                                                    .instanceFunction("getPitch", &LocationAPI::getPitch)
                                                    .instanceFunction("setPitch", &LocationAPI::setPitch)
                                                    .instanceFunction("getYaw", &LocationAPI::getYaw)
                                                    .instanceFunction("setYaw", &LocationAPI::setYaw)

                                                    // LocationAPI extends PositionAPI
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

Local<Value> LocationAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScript("<Location>");
    }
    Catch;
}

Local<Value> LocationAPI::getPitch(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getPitch());
    }
    Catch;
}

Local<Value> LocationAPI::setPitch(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kNumber);
        get()->setPitch(ConvertToCpp<float>(args[0]));
        return Local<Value>();
    }
    Catch;
}

Local<Value> LocationAPI::getYaw(Arguments const& /* args */) {
    try {
        return ConvertToScript(get()->getYaw());
    }
    Catch;
}

Local<Value> LocationAPI::setYaw(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kNumber);
        get()->setYaw(ConvertToCpp<float>(args[0]));
        return Local<Value>();
    }
    Catch;
}


} // namespace jse