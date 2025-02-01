#include "api/command/CommandSenderAPI.h"
#include "api/APIHelper.h"
#include "api/PlayerAPI.h"
#include "converter/Convert.h"
#include "utils/Using.h"
#include <cstddef>
#include <sstream>


namespace jse {

ClassDefine<CommandSenderAPI> CommandSenderAPI::builder =
    defineClass<CommandSenderAPI>("CommandSender")
        .constructor(nullptr)
        .instanceFunction("toString", &CommandSenderAPI::toString)
        .instanceFunction("asCommandSender", &CommandSenderAPI::asCommandSender)
        .instanceFunction("asConsole", &CommandSenderAPI::asConsole)
        .instanceFunction("asActor", &CommandSenderAPI::asActor)
        .instanceFunction("asPlayer", &CommandSenderAPI::asPlayer)
        .instanceFunction("sendMessage", &CommandSenderAPI::sendMessage)
        .instanceFunction("sendErrorMessage", &CommandSenderAPI::sendErrorMessage)
        .instanceFunction("getServer", &CommandSenderAPI::getServer)
        .instanceFunction("getName", &CommandSenderAPI::getName)

        // CommandSenderAPI extends PermissibleAPI
        .instanceFunction("isOp", &PermissibleAPI::isOp)
        .instanceFunction("setOp", &PermissibleAPI::setOp)
        .instanceFunction("isPermissionSet", &PermissibleAPI::isPermissionSet)
        .instanceFunction("hasPermission", &PermissibleAPI::hasPermission)
        .instanceFunction("addAttachment", &PermissibleAPI::addAttachment)
        .instanceFunction("removeAttachment", &PermissibleAPI::removeAttachment)
        .instanceFunction("recalculatePermissions", &PermissibleAPI::recalculatePermissions)
        .instanceFunction("getEffectivePermissions", &PermissibleAPI::getEffectivePermissions)
        .instanceFunction("asCommandSender", &PermissibleAPI::asCommandSender)
        .build();


Local<Value> CommandSenderAPI::toString(Arguments const& /* args */) { return ConvertToScript("<CommandSender>"); }

Local<Value> CommandSenderAPI::asCommandSender(Arguments const& /* args */) {
    return CommandSenderAPI::newInstance(get());
}

Local<Value> CommandSenderAPI::asConsole(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> CommandSenderAPI::asActor(Arguments const& /* args */) {
    if (mSender->asActor() == nullptr) return Local<Value>();
    return ActorAPI::newInstance(mSender->asActor());
}

Local<Value> CommandSenderAPI::asPlayer(Arguments const& /* args */) {
    if (mSender->asPlayer() == nullptr) return Local<Value>();
    return PlayerAPI::newInstance(mSender->asPlayer());
}

Local<Value> CommandSenderAPI::sendMessage(Arguments const& args) {
    // ...args 不限参数
    try {
        std::ostringstream oss;
        for (size_t i = 0; i < args.size(); ++i) ToString(args[i], oss);
        this->mSender->sendMessage(oss.str());
        return Local<Value>();
    }
    Catch;
}

Local<Value> CommandSenderAPI::sendErrorMessage(Arguments const& args) {
    // ...args 不限参数
    try {
        std::ostringstream oss;
        for (size_t i = 0; i < args.size(); ++i) ToString(args[i], oss);
        this->mSender->sendErrorMessage(oss.str());
        return Local<Value>();
    }
    Catch;
}

Local<Value> CommandSenderAPI::getServer(Arguments const& /* args */) { return Local<Value>(); }

Local<Value> CommandSenderAPI::getName(Arguments const& /* args */) {
    try {
        return ConvertToScript(this->mSender->getName());
    }
    Catch;
}


} // namespace jse