#include "api/util/VectorAPI.h"
#include "api/APIHelper.h"
#include "endstone/util/vector.h"
#include "utils/Convert.h"
#include "utils/Defines.h"
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
            endstone::Vector<float>{
                ConvertFromScriptX<float>(args[0]),
                ConvertFromScriptX<float>(args[1]),
                ConvertFromScriptX<float>(args[2])
            }
        ));
    }
    return nullptr;
}

/* instanceProperty */
Local<Value> VectorAPI::GetterX() { return ConvertToScriptX(this->mVector.getX()); }
Local<Value> VectorAPI::GetterY() { return ConvertToScriptX(this->mVector.getY()); }
Local<Value> VectorAPI::GetterZ() { return ConvertToScriptX(this->mVector.getZ()); }

void VectorAPI::SetterX(Local<Value> const& value) { this->mVector.setX(ConvertFromScriptX<float>(value)); }
void VectorAPI::SetterY(Local<Value> const& value) { this->mVector.setY(ConvertFromScriptX<float>(value)); }
void VectorAPI::SetterZ(Local<Value> const& value) { this->mVector.setZ(ConvertFromScriptX<float>(value)); }


/* Methods */
Local<Value> VectorAPI::toString(Arguments const& /* args */) {
    return ConvertToScriptX(fmt::format("<Vector({0}, {1}, {2})>", get().getX(), get().getY(), get().getZ()));
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


Local<Value> VectorAPI::length(Arguments const& /* args */) { return ConvertToScriptX(get().length()); }

Local<Value> VectorAPI::lengthSquared(Arguments const& /* args */) { return ConvertToScriptX(get().lengthSquared()); }

Local<Value> VectorAPI::distance(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kObject);
    try {
        if (!IsInstanceOf<VectorAPI>(args[0])) {
            throw script::Exception("Invalid argument type");
        }
        return ConvertToScriptX(get().distance(GetScriptClass(VectorAPI, args[0])->get()));
    }
    Catch;
}

Local<Value> VectorAPI::distanceSquared(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kObject);
    try {
        if (!IsInstanceOf<VectorAPI>(args[0])) {
            throw script::Exception("Invalid argument type");
        }
        return ConvertToScriptX(get().distanceSquared(GetScriptClass(VectorAPI, args[0])->get()));
    }
    Catch;
}


} // namespace jse