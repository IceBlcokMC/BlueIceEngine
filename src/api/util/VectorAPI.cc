#include "api/util/VectorAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"
#include "endstone/util/vector.h"
#include "utils/Using.h"
#include <memory>


namespace jse {

ClassDefine<VectorAPI> VectorAPI::builder = defineClass<VectorAPI>("Vector")
                                                .constructor(&VectorAPI::make)
                                                .instanceFunction("toString", &VectorAPI::toString)
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


VectorAPI* VectorAPI::make(Arguments const& args) {
    if (args.size() == 3 && args[0].isNumber() && args[1].isNumber() && args[2].isNumber()) {
        return (new VectorAPI(
            args.thiz(),
            std::make_unique<endstone::Vector<float>>(
                ConvertToCpp<float>(args[0]),
                ConvertToCpp<float>(args[1]),
                ConvertToCpp<float>(args[2])
            )
        ));
    }
    return nullptr;
}

/* instanceProperty */
Local<Value> VectorAPI::GetterX() { return ConvertToScript(this->mData->getX()); }
Local<Value> VectorAPI::GetterY() { return ConvertToScript(this->mData->getY()); }
Local<Value> VectorAPI::GetterZ() { return ConvertToScript(this->mData->getZ()); }

void VectorAPI::SetterX(Local<Value> const& value) { this->mData->setX(ConvertToCpp<float>(value)); }
void VectorAPI::SetterY(Local<Value> const& value) { this->mData->setY(ConvertToCpp<float>(value)); }
void VectorAPI::SetterZ(Local<Value> const& value) { this->mData->setZ(ConvertToCpp<float>(value)); }


/* Methods */
Local<Value> VectorAPI::toString(Arguments const& /* args */) {
    return ConvertToScript(fmt::format("<Vector({0}, {1}, {2})>", mData->getX(), mData->getY(), mData->getZ()));
}

Local<Value> VectorAPI::getX(Arguments const& /* args */) { return GetterX(); }
Local<Value> VectorAPI::getY(Arguments const& /* args */) { return GetterY(); }
Local<Value> VectorAPI::getZ(Arguments const& /* args */) { return GetterZ(); }

Local<Value> VectorAPI::setX(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kNumber);
        SetterX(args[0]);
        return Local<Value>();
    }
    Catch;
}
Local<Value> VectorAPI::setY(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kNumber);
        SetterY(args[0]);
        return Local<Value>();
    }
    Catch;
}
Local<Value> VectorAPI::setZ(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kNumber);
        SetterZ(args[0]);
        return Local<Value>();
    }
    Catch;
}


Local<Value> VectorAPI::length(Arguments const& /* args */) { return ConvertToScript(mData->length()); }

Local<Value> VectorAPI::lengthSquared(Arguments const& /* args */) { return ConvertToScript(mData->lengthSquared()); }

Local<Value> VectorAPI::distance(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kObject);
    try {
        if (!IsInstanceOf<VectorAPI>(args[0])) {
            throw script::Exception(ERR_WRONG_ARG_TYPE);
        }
        return ConvertToScript(mData->distance(*GetScriptClass(VectorAPI, args[0])->get()));
    }
    CatchAndThrow;
}

Local<Value> VectorAPI::distanceSquared(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kObject);
    try {
        if (!IsInstanceOf<VectorAPI>(args[0])) {
            throw script::Exception(ERR_WRONG_ARG_TYPE);
        }
        return ConvertToScript(mData->distanceSquared(*GetScriptClass(VectorAPI, args[0])->get()));
    }
    CatchAndThrow;
}


} // namespace jse