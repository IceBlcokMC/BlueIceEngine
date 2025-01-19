#pragma once
#include "api/ColorFormatAPI.h"
#include "api/LoggerAPI.h"
#include "api/PlayerAPI.h"
#include "api/ServerAPI.h"
#include "api/SkinAPI.h"
#include "api/actor/ActorAPI.h"
#include "api/actor/MobAPI.h"
#include "api/command/CommandAPI.h"
#include "api/command/CommandSenderAPI.h"
#include "api/jse/EnumAPI.h"
#include "api/jse/FileAPI.h"
#include "api/jse/JSEAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "api/level/DimensionAPI.h"
#include "api/level/LevelAPI.h"
#include "api/permissions/PermissibleAPI.h"
#include "api/permissions/PermissionAPI.h"
#include "api/plugin/PluginAPI.h"
#include "api/plugin/PluginDescriptionAPI.h"
#include "api/util/SocketAddressAPI.h"
#include "api/util/UUIDAPI.h"
#include "api/util/VectorAPI.h"
#include "utils/Using.h"


namespace jse {

inline void BindAPI(ScriptEngine* engine) {
    EnumAPI::RegisterEnum(engine);

#define STATIC_CLASS(CLASS)   engine->registerNativeClass(CLASS::builder)
#define INSTANCE_CLASS(CLASS) engine->registerNativeClass<CLASS>(CLASS::builder)

    /* actor */
    INSTANCE_CLASS(ActorAPI);
    INSTANCE_CLASS(MobAPI);

    /* command */
    INSTANCE_CLASS(CommandAPI);
    INSTANCE_CLASS(CommandSenderAPI);

    /* jse */
    STATIC_CLASS(JSEAPI);
    STATIC_CLASS(FileAPI);

    /* lang */
    INSTANCE_CLASS(TranslatableAPI);

    /* level */
    INSTANCE_CLASS(LevelAPI);
    INSTANCE_CLASS(DimensionAPI);

    /* permissions */
    INSTANCE_CLASS(PermissibleAPI);
    INSTANCE_CLASS(PermissionAPI);

    /* plugin */
    INSTANCE_CLASS(PluginAPI);
    INSTANCE_CLASS(PluginDescriptionAPI);

    /* util */
    INSTANCE_CLASS(UUIDAPI);
    INSTANCE_CLASS(VectorAPI);
    INSTANCE_CLASS(SocketAddressAPI);

    /* other */
    INSTANCE_CLASS(LoggerAPI);
    INSTANCE_CLASS(PlayerAPI);
    INSTANCE_CLASS(ServerAPI);
    INSTANCE_CLASS(SkinAPI);
    INSTANCE_CLASS(SkinImageDataAPI);
    STATIC_CLASS(ColorFormatAPI);

#undef STATIC_CLASS
#undef INSTANCE_CLASS
}


} // namespace jse
