#include "LevelAPI.h"
#include "DimensionAPI.h"
#include "api/APIHelper.h"
#include "api/actor/ActorAPI.h"
#include "utils/Convert.h"


namespace jse {

ClassDefine<LevelAPI> LevelAPI::builder = defineClass<LevelAPI>("Level")
                                              .constructor(nullptr)
                                              .instanceFunction("toString", &LevelAPI::toString)
                                              .instanceFunction("getName", &LevelAPI::getName)
                                              .instanceFunction("getActors", &LevelAPI::getActors)
                                              .instanceFunction("getTime", &LevelAPI::getTime)
                                              .instanceFunction("setTime", &LevelAPI::setTime)
                                              .instanceFunction("getActors", &LevelAPI::getActors)
                                              .instanceFunction("getDimensions", &LevelAPI::getDimensions)
                                              .instanceFunction("getDimension", &LevelAPI::getDimension)
                                              .build();

Local<Value> LevelAPI::toString(Arguments const& /* args */) { return ConvertToScriptX("<Level>"); }

Local<Value> LevelAPI::getName(Arguments const& /* args */) { return ConvertToScriptX(get()->getName()); }

Local<Value> LevelAPI::getActors(Arguments const& /* args */) {
    try {
        auto actors = get()->getActors();
        auto res    = Array::newArray(actors.size());
        for (std::size_t i = 0; i < actors.size(); i++) {
            res.set(i, ActorAPI::newActorAPI(actors[i]));
        }
        return res;
    }
    Catch;
}

Local<Value> LevelAPI::getTime(Arguments const& /* args */) { return Number::newNumber(get()->getTime()); }

Local<Value> LevelAPI::setTime(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kNumber);
        get()->setTime(args[0].asNumber().toInt32());
        return Local<Value>();
    }
    Catch;
}

Local<Value> LevelAPI::getDimensions(Arguments const& /* args */) {
    try {
        auto dimensions = get()->getDimensions();
        auto res        = Array::newArray(dimensions.size());
        for (std::size_t i = 0; i < dimensions.size(); i++) {
            res.set(i, DimensionAPI::newDimensionAPI(dimensions[i]));
        }
        return res;
    }
    Catch;
}

Local<Value> LevelAPI::getDimension(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kString);
        return DimensionAPI::newDimensionAPI(get()->getDimension(ConvertFromScriptX<std::string>(args[0])));
    }
    Catch;
}


} // namespace jse