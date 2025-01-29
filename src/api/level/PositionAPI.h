#pragma once
#include "api/util/VectorAPI.h"
#include "endstone/level/position.h"
#include "utils/Defines.h"
#include "utils/Using.h"


namespace jse {


class PositionAPI : public VectorAPI {
    endstone::Position* mPosition;

public:
    template <typename T>
    explicit PositionAPI(endstone::Position* pos, ScriptClass::ConstructFromCpp<T> class_)
    : VectorAPI(*pos, class_),
      mPosition(pos) {}

    explicit PositionAPI(endstone::Position* pos) : VectorAPI(*pos, ConstructFromCpp<PositionAPI>{}), mPosition(pos) {}

    static Local<Object> newInstance(endstone::Position* pos) { return (new PositionAPI(pos))->getScriptObject(); }

    endstone::Position* get() { return mPosition; }

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