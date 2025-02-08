#pragma once
#include "api/APIHelper.h"
#include "api/form/FormAPI.h"
#include "endstone/form/action_form.h"
#include "utils/Using.h"


namespace jse {


class ActionFormAPI : public FormAPI<endstone::ActionForm>, public ScriptClass {
    endstone::ActionForm mActionForm;

public:
    explicit ActionFormAPI(Local<Object> const& thiz) : FormAPI(mActionForm), ScriptClass(thiz) {}

    static ActionFormAPI* make(Arguments const& args);

    endstone::ActionForm& get() { return mActionForm; }

public:
    METHODS(toString);
    METHODS(getContent);
    METHODS(setContent);
    METHODS(addButton);
    // METHODS(getButtons);
    // METHODS(setButtons);
    // METHODS(getOnSubmit);
    METHODS(setOnSubmit);

public:
    static ClassDefine<ActionFormAPI> builder;
};


} // namespace jse