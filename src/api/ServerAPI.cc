#include "api/ServerAPI.h"
#include "APIHelper.h"
#include "api/APIHelper.h"
#include "api/LoggerAPI.h"
#include "api/PlayerAPI.h"
#include "api/command/CommandSenderAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "api/util/UUIDAPI.h"
#include "endstone/player.h"
#include "endstone/server.h"
#include "level/LevelAPI.h"
#include "utils/Defines.h"


namespace jse {

ClassDefine<ServerAPI> ServerAPI::builder =
    defineClass<ServerAPI>("Server")
        .constructor(nullptr)
        .instanceFunction("toString", &ServerAPI::toString)
        .instanceFunction("getName", &ServerAPI::getName)
        .instanceFunction("getVersion", &ServerAPI::getVersion)
        .instanceFunction("getMinecraftVersion", &ServerAPI::getMinecraftVersion)
        .instanceFunction("getLogger", &ServerAPI::getLogger)
        .instanceFunction("getLanguage", &ServerAPI::getLanguage)
        .instanceFunction("getPluginManager", &ServerAPI::getPluginManager)
        .instanceFunction("getPluginCommand", &ServerAPI::getPluginCommand)
        .instanceFunction("getCommandSender", &ServerAPI::getCommandSender)
        .instanceFunction("dispatchCommand", &ServerAPI::dispatchCommand)
        .instanceFunction("getScheduler", &ServerAPI::getScheduler)
        .instanceFunction("getLevel", &ServerAPI::getLevel)
        .instanceFunction("getOnlinePlayers", &ServerAPI::getOnlinePlayers)
        .instanceFunction("getMaxPlayers", &ServerAPI::getMaxPlayers)
        .instanceFunction("setMaxPlayers", &ServerAPI::setMaxPlayers)
        .instanceFunction("getPlayer", &ServerAPI::getPlayer)
        .instanceFunction("getOnlineMode", &ServerAPI::getOnlineMode)
        .instanceFunction("shutdown", &ServerAPI::shutdown)
        .instanceFunction("reload", &ServerAPI::reload)
        .instanceFunction("reloadData", &ServerAPI::reloadData)
        .instanceFunction("broadcast", &ServerAPI::broadcast)
        .instanceFunction("broadcastMessage", &ServerAPI::broadcastMessage)
        .instanceFunction("isPrimaryThread", &ServerAPI::isPrimaryThread)
        .instanceFunction("getScoreboard", &ServerAPI::getScoreboard)
        .instanceFunction("createScoreboard", &ServerAPI::createScoreboard)
        .instanceFunction("getCurrentMillisecondsPerTick", &ServerAPI::getCurrentMillisecondsPerTick)
        .instanceFunction("getAverageMillisecondsPerTick", &ServerAPI::getAverageMillisecondsPerTick)
        .instanceFunction("getCurrentTicksPerSecond", &ServerAPI::getCurrentTicksPerSecond)
        .instanceFunction("getAverageTicksPerSecond", &ServerAPI::getAverageTicksPerSecond)
        .instanceFunction("getCurrentTickUsage", &ServerAPI::getCurrentTickUsage)
        .instanceFunction("getAverageTickUsage", &ServerAPI::getAverageTickUsage)
        .instanceFunction("getStartTime", &ServerAPI::getStartTime)
        .instanceFunction("createBossBar", &ServerAPI::createBossBar)
        .instanceFunction("createBlockData", &ServerAPI::createBlockData)
        .instanceFunction("getBanList", &ServerAPI::getBanList)
        .instanceFunction("getIpBanList", &ServerAPI::getIpBanList)
        .property("BroadcastChannelAdmin", []() { return ConvertToScript(endstone::Server::BroadcastChannelAdmin); })
        .property("BroadcastChannelUser", []() { return ConvertToScript(endstone::Server::BroadcastChannelUser); })
        .build();

Local<Value> ServerAPI::toString(Arguments const& /* args */) { return ConvertToScript("<Server>"); }

Local<Value> ServerAPI::getName(Arguments const& /* args */) { return ConvertToScript(get()->getName()); }

Local<Value> ServerAPI::getVersion(Arguments const& /* args */) { return ConvertToScript(get()->getVersion()); }

Local<Value> ServerAPI::getMinecraftVersion(Arguments const& /* args */) {
    return ConvertToScript(get()->getMinecraftVersion());
}

Local<Value> ServerAPI::getLogger(Arguments const& /* args */) { return LoggerAPI::newInstance(&get()->getLogger()); }

Local<Value> ServerAPI::getLanguage(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getPluginManager(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getPluginCommand(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getCommandSender(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::dispatchCommand(Arguments const& args) try {
    CheckArgsCount(args, 2);
    CheckArgType(args[0], ValueKind::kObject);
    CheckArgType(args[1], ValueKind::kString);
    return ConvertToScript(
        get()->dispatchCommand(*GetScriptClass(CommandSenderAPI, args[0])->get(), ConvertToCpp<std::string>(args[1]))
    );
}
Catch;

Local<Value> ServerAPI::getScheduler(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getLevel(Arguments const& /* args */) { return LevelAPI::newInstance(get()->getLevel()); }

Local<Value> ServerAPI::getOnlinePlayers(Arguments const& /* args */) {
    auto result = Array::newArray();
    for (auto* player : get()->getOnlinePlayers()) {
        result.add(PlayerAPI::newInstance(player));
    }
    return result;
}

Local<Value> ServerAPI::getMaxPlayers(Arguments const& /* args */) { return ConvertToScript(get()->getMaxPlayers()); }

Local<Value> ServerAPI::setMaxPlayers(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kNumber);
    get()->setMaxPlayers(ConvertToCpp<int>(args[0]));
    return Local<Value>();
}

Local<Value> ServerAPI::getPlayer(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        endstone::Player* player{nullptr};
        if (args[0].isString()) {
            player = get()->getPlayer(ConvertToCpp<std::string>(args[0]));
        } else if (args[0].isObject() && IsInstanceOf<UUIDAPI>(args[0])) {
            player = get()->getPlayer(GetScriptClass(UUIDAPI, args[0])->get());
        } else {
            throw script::Exception("Invalid argument type");
        }
        return player ? PlayerAPI::newInstance(player) : Local<Value>();
    }
    Catch;
}

Local<Value> ServerAPI::getOnlineMode(Arguments const& /* args */) { return ConvertToScript(get()->getOnlineMode()); }

Local<Value> ServerAPI::shutdown(Arguments const& /* args */) {
    get()->shutdown();
    return Local<Value>();
}

Local<Value> ServerAPI::reload(Arguments const& /* args */) {
    get()->reload();
    return Local<Value>();
}

Local<Value> ServerAPI::reloadData(Arguments const& /* args */) {
    get()->reloadData();
    return Local<Value>();
}

Local<Value> ServerAPI::broadcast(Arguments const& args) {
    try {
        CheckArgsCount(args, 2);
        // CheckArgType(args[0], ValueKind::kString);
        CheckArgType(args[1], ValueKind::kString);
        if (args[0].isString()) {
            get()->broadcast(ConvertToCpp<std::string>(args[0]), ConvertToCpp<std::string>(args[1]));
        } else if (args[0].isObject() && IsInstanceOf<TranslatableAPI>(args[0])) {
            get()->broadcast(GetScriptClass(TranslatableAPI, args[0])->get(), ConvertToCpp<std::string>(args[1]));
        } else {
            throw script::Exception("Invalid argument type");
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> ServerAPI::broadcastMessage(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kString);
    get()->broadcastMessage(ConvertToCpp<std::string>(args[0]));
    return Local<Value>();
}

Local<Value> ServerAPI::isPrimaryThread(Arguments const& /* args */) {
    return ConvertToScript(get()->isPrimaryThread());
}

Local<Value> ServerAPI::getScoreboard(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::createScoreboard(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getCurrentMillisecondsPerTick(Arguments const& /* args */) {
    return ConvertToScript(get()->getCurrentMillisecondsPerTick());
}

Local<Value> ServerAPI::getAverageMillisecondsPerTick(Arguments const& /* args */) {
    return ConvertToScript(get()->getAverageMillisecondsPerTick());
}

Local<Value> ServerAPI::getCurrentTicksPerSecond(Arguments const& /* args */) {
    return ConvertToScript(get()->getCurrentTicksPerSecond());
}

Local<Value> ServerAPI::getAverageTicksPerSecond(Arguments const& /* args */) {
    return ConvertToScript(get()->getAverageTicksPerSecond());
}

Local<Value> ServerAPI::getCurrentTickUsage(Arguments const& /* args */) {
    return ConvertToScript(get()->getCurrentTickUsage());
}

Local<Value> ServerAPI::getAverageTickUsage(Arguments const& /* args */) {
    return ConvertToScript(get()->getAverageTickUsage());
}

Local<Value> ServerAPI::getStartTime(Arguments const& /* args */) {
    return ConvertToScript(get()->getStartTime().time_since_epoch().count());
}

Local<Value> ServerAPI::createBossBar(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::createBlockData(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getBanList(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> ServerAPI::getIpBanList(Arguments const& /* args */) { return Local<Value>(); }

} // namespace jse