#pragma once
#include "api/APIHelper.h"
#include "api/util/VectorAPI.h"
#include "endstone/level/position.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"
#include <memory>


namespace jse {


class PositionAPI : public VectorAPI {
    SafePointerHolder<endstone::Position> mData;

public:
    explicit PositionAPI(endstone::Position* pos) : VectorAPI(pos), mData(pos) {
        this->_ReConstructScriptClass(ConstructFromCpp<PositionAPI>{});
    }

    explicit PositionAPI(Local<Object> const& thiz, std::unique_ptr<endstone::Position> pos)
    : VectorAPI(pos.get()),
      mData(std::move(pos)) {
        this->_ReConstructScriptClass(thiz);
    }

public:
    static Local<Object> newInstance(endstone::Position* pos) { return (new PositionAPI(pos))->getScriptObject(); }

    static PositionAPI* make(Arguments const& args);

    endstone::Position* get() { return mData.get(); }

public:
    METHODS(toString);
    METHODS(getDimension);
    METHODS(setDimension);
    METHODS(getBlockX);
    METHODS(getBlockY);
    METHODS(getBlockZ);

public:
    static ClassDefine<PositionAPI> builder;
};


} // namespace jse