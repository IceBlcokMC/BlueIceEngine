#pragma once
#include "endstone/util/socket_address.h"
#include "utils/Defines.h"
#include "utils/Using.h"


namespace jse {


class SocketAddressAPI : public ScriptClass {
    endstone::SocketAddress mSocketAddress;

public:
    // C++ new
    explicit SocketAddressAPI(endstone::SocketAddress socketAddress)
    : ScriptClass(ConstructFromCpp<SocketAddressAPI>{}),
      mSocketAddress(std::move(socketAddress)) {}

    static Local<Object> newInstance(endstone::SocketAddress socketAddress) {
        return (new SocketAddressAPI(std::move(socketAddress)))->getScriptObject();
    }

    // Js new
    explicit SocketAddressAPI(Local<Object> const& thiz, endstone::SocketAddress socketAddress)
    : ScriptClass(thiz),
      mSocketAddress(std::move(socketAddress)) {}

    static SocketAddressAPI* make(Arguments const& args);

    endstone::SocketAddress& get() { return mSocketAddress; }

public:
    METHODS(toString);
    METHODS(getHostname);
    METHODS(getPort);

public:
    static ClassDefine<SocketAddressAPI> builder;
};


} // namespace jse