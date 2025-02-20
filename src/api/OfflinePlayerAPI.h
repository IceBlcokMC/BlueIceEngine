#pragma once

#include "api/APIHelper.h"
#include "endstone/offline_player.h"
#include "endstone/player.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"
namespace jse {


class OfflinePlayerAPI : public ScriptClass {
    SafePointerHolder<endstone::OfflinePlayer> mData;

public:
    explicit OfflinePlayerAPI(endstone::OfflinePlayer* data)
    : ScriptClass(ConstructFromCpp<OfflinePlayerAPI>{}),
      mData(data) {}

    Local<Object> newInstance(endstone::OfflinePlayer* data) { return (new OfflinePlayerAPI(data))->getScriptObject(); }

    endstone::OfflinePlayer* get() { return mData.get(); }

public:
    METHODS(toString);
    METHODS(getName);
    METHODS(getUniqueId);

public:
    static ClassDefine<OfflinePlayerAPI> builder;
};


} // namespace jse