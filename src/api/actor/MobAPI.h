#pragma once
#include "api/APIHelper.h"
#include "api/actor/ActorAPI.h"
#include "utils/Using.h"
#include <endstone/actor/mob.h>


namespace jse {

class MobAPI : public ActorAPI {
    endstone::Mob* mMob;

public:
    template <typename T>
    explicit MobAPI(endstone::Mob* mob, ScriptClass::ConstructFromCpp<T> tag) : ActorAPI(mob, tag),
                                                                                mMob(mob) {}

    explicit MobAPI(endstone::Mob* mob) : ActorAPI(mob, ScriptClass::ConstructFromCpp<MobAPI>{}), mMob(mob) {}

    static Local<Object> newInstance(endstone::Mob* mob) { return (new MobAPI(mob))->getScriptObject(); }

    endstone::Mob* get() { return mMob; }

public:
    METHODS(toString);
    METHODS(asMob);
    METHODS(isGliding);

public:
    static ClassDefine<MobAPI> builder;
};


} // namespace jse