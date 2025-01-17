#pragma once
#include "endstone/level/level.h"
#include "utils/Defines.h"
#include "utils/Using.h"


namespace jse {
    

class LevelAPI : public ScriptClass {
    endstone::Level* mLevel;

public:
    explicit LevelAPI(endstone::Level* Level) : ScriptClass(ScriptClass::ConstructFromCpp<LevelAPI>{}), mLevel(Level) {}

    static Local<Object> newLevelAPI(endstone::Level* Level) { return (new LevelAPI(Level))->getScriptObject(); }

    endstone::Level*     get() { return mLevel; }

public:
    METHODS(toString);
    METHODS(getName);
    METHODS(getActors);
    METHODS(getTime);
    METHODS(setTime);
    METHODS(getDimensions);
    METHODS(getDimension);

public:
    static ClassDefine<LevelAPI> builder;
};


} // namespace jse