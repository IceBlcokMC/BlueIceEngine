#pragma once
#include "endstone/level/dimension.h"
#include "utils/Defines.h"
#include "utils/Using.h"


namespace jse {


class DimensionAPI : public ScriptClass {
    endstone::Dimension* mDimension;

public:
    explicit DimensionAPI(endstone::Dimension* dimension)
    : ScriptClass(ScriptClass::ConstructFromCpp<DimensionAPI>{}),
      mDimension(dimension) {}

    static Local<Object> newDimensionAPI(endstone::Dimension* dimension) {
        return (new DimensionAPI(dimension))->getScriptObject();
    }

    endstone::Dimension* get() { return mDimension; }

public:
    METHODS(toString);
    METHODS(getName);
    METHODS(getType);
    METHODS(getLevel);
    METHODS(getBlockAt); // TODO: BlockAPI

public:
    static ClassDefine<DimensionAPI> builder;
};


} // namespace jse