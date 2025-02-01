#include "api/util/SocketAddressAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"
#include "endstone/util/socket_address.h"
#include "utils/Using.h"
#include <cstdint>


namespace jse {

ClassDefine<SocketAddressAPI> SocketAddressAPI::builder =
    defineClass<SocketAddressAPI>("SocketAddress")
        .constructor(&SocketAddressAPI::make)
        .instanceFunction("toString", &SocketAddressAPI::toString)
        .instanceFunction("getHostname", &SocketAddressAPI::getHostname)
        .instanceFunction("getPort", &SocketAddressAPI::getPort)
        .build();


SocketAddressAPI* SocketAddressAPI::make(Arguments const& args) {
    if (args.size() == 2 && args[0].isString() && args[1].isNumber()) {
        return new SocketAddressAPI(
            args.thiz(),
            endstone::SocketAddress{ConvertToCpp<string>(args[0]), static_cast<uint32_t>(ConvertToCpp<int>(args[1]))}
        );
    }
    return nullptr;
}

Local<Value> SocketAddressAPI::toString(Arguments const& /* args */) { return ConvertToScript("<SocketAddress>"); }

Local<Value> SocketAddressAPI::getHostname(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getHostname());
    }
    Catch;
}

Local<Value> SocketAddressAPI::getPort(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getPort());
    }
    Catch;
}


} // namespace jse