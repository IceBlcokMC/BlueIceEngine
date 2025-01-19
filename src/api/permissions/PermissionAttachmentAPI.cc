#include "api/permissions/PermissionAttachmentAPI.h"
#include "api/APIHelper.h"
#include "api/permissions/PermissibleAPI.h"
#include "api/permissions/PermissionAPI.h"
#include "api/plugin/PluginAPI.h"
#include "endstone/permissions/permission_attachment.h"
#include "utils/Convert.h"
#include "utils/Defines.h"
#include "utils/Using.h"

namespace jse {

ClassDefine<PermissionAttachmentAPI> PermissionAttachmentAPI::builder =
    defineClass<PermissionAttachmentAPI>("PermissionAttachment")
        .constructor(nullptr)
        .instanceFunction("toString", &PermissionAttachmentAPI::toString)
        .instanceFunction("getPlugin", &PermissionAttachmentAPI::getPlugin)
        .instanceFunction("setRemovalCallback", &PermissionAttachmentAPI::setRemovalCallback)
        .instanceFunction("getRemovalCallback", &PermissionAttachmentAPI::getRemovalCallback)
        .instanceFunction("getPermissible", &PermissionAttachmentAPI::getPermissible)
        .instanceFunction("getPermissions", &PermissionAttachmentAPI::getPermissions)
        .instanceFunction("setPermission", &PermissionAttachmentAPI::setPermission)
        .instanceFunction("unsetPermission", &PermissionAttachmentAPI::unsetPermission)
        .instanceFunction("remove", &PermissionAttachmentAPI::remove)
        .build();

Local<Value> PermissionAttachmentAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScriptX("<PermissionAttachment>");
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::getPlugin(Arguments const& /* args */) {
    try {
        return PluginAPI::newPluginAPI(&get()->getPlugin());
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::setRemovalCallback(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kFunction);
        auto fn = script::Global(args[0].asFunction());
        get()->setRemovalCallback([fn{std::move(fn)}](endstone::PermissionAttachment const& attachment) {
            if (!fn.isEmpty()) {
                fn.get().call(PermissionAttachmentAPI::newPermissionAttachmentAPI(
                    &const_cast<endstone::PermissionAttachment&>(attachment)
                ));
            }
        });
        return Local<Value>();
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::getRemovalCallback(Arguments const& /* args */) {
    try {
        auto fn = get()->getRemovalCallback();
        if (fn) {
            return Function::newFunction([fn{std::move(fn)}](Arguments const& args) {
                try {
                    CheckArgsCount(args, 1);
                    if (fn && args[0].isObject() && IsInstanceOf<PermissionAttachmentAPI>(args[0])) {
                        fn(*GetScriptClass(PermissionAttachmentAPI, args[0])->get());
                    }
                    return Local<Value>();
                }
                Catch;
            });
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::getPermissible(Arguments const& /* args */) {
    try {
        return PermissibleAPI::newPermissibleAPI(&get()->getPermissible());
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::getPermissions(Arguments const& /* args */) {
    try {
        auto res = get()->getPermissions();
        return ConvertToScriptX(std::move(res));
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::setPermission(Arguments const& args) {
    try {
        CheckArgsCount(args, 2);
        CheckArgType(args[1], ValueKind::kBoolean);
        if (args[0].isString()) {
            get()->setPermission(ConvertFromScriptX<string>(args[0]), args[1].asBoolean().value());
        } else if (args[0].isObject() && IsInstanceOf<PermissionAPI>(args[0])) {
            get()->setPermission(*GetScriptClass(PermissionAPI, args[0])->get(), args[1].asBoolean().value());
        } else {
            throw script::Exception("Invalid permission argument");
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::unsetPermission(Arguments const& args) {
    try {
        CheckArgsCount(args, 1);
        if (args[0].isString()) {
            get()->unsetPermission(ConvertFromScriptX<string>(args[0]));
        } else if (args[0].isObject() && IsInstanceOf<PermissionAPI>(args[0])) {
            get()->unsetPermission(*GetScriptClass(PermissionAPI, args[0])->get());
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> PermissionAttachmentAPI::remove(Arguments const& /* args */) {
    try {
        this->get()->remove();
        return Local<Value>();
    }
    Catch;
}


} // namespace jse