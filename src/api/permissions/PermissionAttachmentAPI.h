#pragma once
#include "endstone/permissions/permission_attachment.h"
#include "utils/Defines.h"
#include "utils/Using.h"

namespace jse {


class PermissionAttachmentAPI : public ScriptClass {
    endstone::PermissionAttachment* mPermissionAttachment;

public:
    explicit PermissionAttachmentAPI(endstone::PermissionAttachment* permattachment)
    : ScriptClass(ScriptClass::ConstructFromCpp<PermissionAttachmentAPI>{}),
      mPermissionAttachment(permattachment) {}

    static Local<Object> newInstance(endstone::PermissionAttachment* permattachment) {
        return (new PermissionAttachmentAPI{permattachment})->getScriptObject();
    }

    endstone::PermissionAttachment* get() { return mPermissionAttachment; }

public:
    METHODS(toString);
    METHODS(getPlugin);
    METHODS(setRemovalCallback);
    METHODS(getRemovalCallback);
    METHODS(getPermissible);
    METHODS(getPermissions);
    METHODS(setPermission);
    METHODS(unsetPermission);
    METHODS(remove);

public:
    static ClassDefine<PermissionAttachmentAPI> builder;
};


} // namespace jse