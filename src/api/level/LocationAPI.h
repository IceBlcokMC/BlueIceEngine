#pragma once
#include "api/level/PositionAPI.h"
#include "endstone/level/location.h"
#include "utils/Defines.h"
#include "utils/Using.h"


namespace jse {


class LocationAPI : public PositionAPI {
    endstone::Location* mLocation;

public:
    template <typename T>
    explicit LocationAPI(endstone::Location* location, ScriptClass::ConstructFromCpp<T> class_)
    : PositionAPI(location, class_),
      mLocation(location) {}

    explicit LocationAPI(endstone::Location* location)
    : PositionAPI(location, ConstructFromCpp<LocationAPI>{}),
      mLocation(location) {}

    static Local<Object> newInstance(endstone::Location* pos) { return (new LocationAPI(pos))->getScriptObject(); }

    endstone::Location* get() { return mLocation; }

public:
    METHODS(toString);
    METHODS(getPitch);
    METHODS(setPitch);
    METHODS(getYaw);
    METHODS(setYaw);

public:
    static ClassDefine<LocationAPI> builder;
};


} // namespace jse