#pragma once
#include "api/APIHelper.h"
#include "api/level/PositionAPI.h"
#include "endstone/level/location.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"


namespace jse {


class LocationAPI : public PositionAPI {
    SafePointerHolder<endstone::Location> mData;

public:
    explicit LocationAPI(endstone::Location* loc) : PositionAPI(loc), mData(loc) {
        this->_ReConstructScriptClass(ConstructFromCpp<LocationAPI>{});
    }

    explicit LocationAPI(Local<Object> const& thiz, std::unique_ptr<endstone::Location> loc)
    : PositionAPI(loc.get()),
      mData(std::move(loc)) {
        this->_ReConstructScriptClass(thiz);
    }

public:
    static Local<Object> newInstance(endstone::Location* pos) { return (new LocationAPI(pos))->getScriptObject(); }

    static LocationAPI* make(Arguments const& args);

    endstone::Location* get() { return mData.get(); }

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