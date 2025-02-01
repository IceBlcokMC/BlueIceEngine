#pragma once
#include "FormAPI.h"
#include "api/APIHelper.h"
#include "endstone/form/message_form.h"
#include "utils/Using.h"



namespace jse {

class MessageFormAPI : public FormAPI<endstone::MessageForm>, public ScriptClass {
    endstone::MessageForm mMessageForm;

public:
    explicit MessageFormAPI(Local<Object> const& thiz) : FormAPI(mMessageForm), ScriptClass(thiz) {}

    static MessageFormAPI* make(Arguments const& args);

    endstone::MessageForm& get() { return mMessageForm; }

public:
    METHODS(toString);
    METHODS(getContent);
    METHODS(setContent);
    METHODS(getButton1);
    METHODS(setButton1);
    METHODS(getButton2);
    METHODS(setButton2);
    METHODS(setOnSubmit);
    // METHODS(getOnSubmit);

public:
    static ClassDefine<MessageFormAPI> builder;
};


} // namespace jse