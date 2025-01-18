#pragma once
#include "api/ColorFormatAPI.h"
#include "api/LoggerAPI.h"
#include "api/PlayerAPI.h"
#include "api/ServerAPI.h"
#include "api/actor/ActorAPI.h"
#include "api/actor/MobAPI.h"
#include "api/command/CommandAPI.h"
#include "api/command/CommandSenderAPI.h"
#include "api/jse/EnumAPI.h"
#include "api/jse/FileAPI.h"
#include "api/jse/JSEAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "api/level/LevelAPI.h"
#include "api/permissions/PermissibleAPI.h"
#include "api/permissions/PermissionAPI.h"
#include "api/plugin/PluginAPI.h"
#include "api/plugin/PluginDescriptionAPI.h"
#include "api/util/UUIDAPI.h"
#include "api/util/VectorAPI.h"
#include "utils/Using.h"


namespace jse {

inline void BindAPI(ScriptEngine* engine) {

    // enum class
    EnumAPI::RegisterEnum(engine);

    // static class
    engine->registerNativeClass(JSEAPI::builder);
    engine->registerNativeClass(FileAPI::builder);
    engine->registerNativeClass(ColorFormatAPI::builder);

    // instance class
#define REGISTER_CLASS(CLASS) engine->registerNativeClass<CLASS>(CLASS::builder)

    REGISTER_CLASS(PluginAPI);
    REGISTER_CLASS(PluginDescriptionAPI);

    REGISTER_CLASS(TranslatableAPI);

    REGISTER_CLASS(LevelAPI);

    REGISTER_CLASS(PermissibleAPI);
    REGISTER_CLASS(PermissionAPI);

    REGISTER_CLASS(CommandAPI);
    REGISTER_CLASS(CommandSenderAPI);

    REGISTER_CLASS(ActorAPI);
    REGISTER_CLASS(MobAPI);

    REGISTER_CLASS(LoggerAPI);
    REGISTER_CLASS(PlayerAPI);

    REGISTER_CLASS(UUIDAPI);
    REGISTER_CLASS(VectorAPI);
    REGISTER_CLASS(ServerAPI);

#undef REGISTER_CLASS
}


} // namespace jse
