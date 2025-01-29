#include "api/permissions/PermissionAttachmentInfoAPI.h"
#include "api/APIHelper.h"
#include "api/permissions/PermissibleAPI.h"
#include "api/permissions/PermissionAttachmentAPI.h"
#include "utils/Convert.h"
#include "utils/Defines.h"
#include "utils/Using.h"

namespace jse {

ClassDefine<PermissionAttachmentInfoAPI> PermissionAttachmentInfoAPI::builder =
    defineClass<PermissionAttachmentInfoAPI>("PermissionAttachmentInfo")
        .constructor(nullptr)
        .instanceFunction("toString", &PermissionAttachmentInfoAPI::toString)
        .instanceFunction("getPermissible", &PermissionAttachmentInfoAPI::getPermissible)
        .instanceFunction("getPermission", &PermissionAttachmentInfoAPI::getPermission)
        .instanceFunction("getAttachment", &PermissionAttachmentInfoAPI::getAttachment)
        .instanceFunction("getValue", &PermissionAttachmentInfoAPI::getValue)
        .build();

Local<Value> PermissionAttachmentInfoAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScriptX("<PermissionAttachmentInfo>");
    }
    Catch;
}

Local<Value> PermissionAttachmentInfoAPI::getPermissible(Arguments const& /* args */) {
    try {
        return PermissibleAPI::newInstance(&get()->getPermissible());
    }
    Catch;
}

Local<Value> PermissionAttachmentInfoAPI::getPermission(Arguments const& /* args */) {
    try {
        return ConvertToScriptX(get()->getPermission());
    }
    Catch;
}

Local<Value> PermissionAttachmentInfoAPI::getAttachment(Arguments const& /* args */) {
    try {
        auto attachment = get()->getAttachment();
        if (attachment == nullptr) {
            return Local<Value>();
        }
        return PermissionAttachmentAPI::newInstance(attachment);
    }
    Catch;
}

Local<Value> PermissionAttachmentInfoAPI::getValue(Arguments const& /* args */) {
    try {
        return ConvertToScriptX(get()->getValue());
    }
    Catch;
}


} // namespace jse
