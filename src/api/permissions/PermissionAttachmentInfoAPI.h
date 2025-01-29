#pragma once
#include "endstone/permissions/permission_attachment_info.h"
#include "utils/Defines.h"
#include "utils/Using.h"

namespace jse {


class PermissionAttachmentInfoAPI : public ScriptClass {
    endstone::PermissionAttachmentInfo* mPermissionAttachmentInfo;

public:
    explicit PermissionAttachmentInfoAPI(endstone::PermissionAttachmentInfo* permattachmentInfo)
    : ScriptClass(ConstructFromCpp<PermissionAttachmentInfoAPI>{}),
      mPermissionAttachmentInfo(permattachmentInfo) {}

    static Local<Object> newInstance(endstone::PermissionAttachmentInfo* permattachmentInfo) {
        return (new PermissionAttachmentInfoAPI{permattachmentInfo})->getScriptObject();
    }

    endstone::PermissionAttachmentInfo* get() { return mPermissionAttachmentInfo; }

public:
    METHODS(toString);
    METHODS(getPermissible);
    METHODS(getPermission);
    METHODS(getAttachment);
    METHODS(getValue);

public:
    static ClassDefine<PermissionAttachmentInfoAPI> builder;
};


} // namespace jse