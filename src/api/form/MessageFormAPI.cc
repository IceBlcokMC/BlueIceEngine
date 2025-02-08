#include "MessageFormAPI.h"
#include "api/APIHelper.h"
#include "api/PlayerAPI.h"
#include "api/form/FormAPI.h"
#include "converter/Convert.h"
#include "endstone/endstone.h"
#include "endstone/message.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"
#include <variant>


namespace jse {

ClassDefine<MessageFormAPI> MessageFormAPI::builder = defineClass<MessageFormAPI>("MessageForm")
                                                          .constructor(&MessageFormAPI::make)
                                                          .instanceFunction("toString", &MessageFormAPI::toString)
                                                          .instanceFunction("getContent", &MessageFormAPI::getContent)
                                                          .instanceFunction("setContent", &MessageFormAPI::setContent)
                                                          .instanceFunction("getButton1", &MessageFormAPI::getButton1)
                                                          .instanceFunction("setButton1", &MessageFormAPI::setButton1)
                                                          .instanceFunction("getButton2", &MessageFormAPI::getButton2)
                                                          .instanceFunction("setButton2", &MessageFormAPI::setButton2)
                                                          .instanceFunction("setOnSubmit", &MessageFormAPI::setOnSubmit)

                                                          // MessageFormAPI extends FormAPI
                                                          .instanceFunction("getTitle", &MessageFormAPI::getTitle)
                                                          .instanceFunction("setTitle", &MessageFormAPI::setTitle)
                                                          .instanceFunction("setOnClose", &MessageFormAPI::setOnClose)
                                                          .build();

MessageFormAPI* MessageFormAPI::make(Arguments const& args) { return new MessageFormAPI(args.thiz()); }


Local<Value> MessageFormAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScript("<MessageForm>");
    }
    Catch;
}

Local<Value> MessageFormAPI::getContent(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getContent());
    }
    Catch;
}

Local<Value> MessageFormAPI::setContent(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        mMessageForm.setContent(ConvertToCpp<endstone::Message>(args[0]));
        return args.thiz();
    }
    CatchAndThrow;
}

Local<Value> MessageFormAPI::getButton1(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getButton1());
    }
    Catch;
}

Local<Value> MessageFormAPI::setButton1(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        mMessageForm.setButton1(ConvertToCpp<endstone::Message>(args[0]));
        return args.thiz();
    }
    CatchAndThrow;
}

Local<Value> MessageFormAPI::getButton2(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getButton2());
    }
    Catch;
}

Local<Value> MessageFormAPI::setButton2(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        mMessageForm.setButton2(ConvertToCpp<endstone::Message>(args[0]));
        return args.thiz();
    }
    CatchAndThrow;
}

Local<Value> MessageFormAPI::setOnSubmit(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kFunction);
    try {
        auto           engine = EngineScope::currentEngine();
        script::Global callback{args[0].asFunction()};

        auto ptr = SafeTransfer<Function>::make(engine, std::move(callback));

        get().setOnSubmit([ptr](endstone::Player* player, int idk) {
            if (ptr) {
                EngineScope enter{ptr->mEngine};
                try {
                    if (player) ptr->mGlobal.get().call({}, PlayerAPI::newInstance(player), ConvertToScript(idk));
                    else ptr->mGlobal.get().call({}, Local<Value>{}, ConvertToScript(idk));
                }
                CatchNotReturn;
            }
        });
        return args.thiz();
    }
    Catch;
}

// Local<Value> MessageFormAPI::getOnSubmit(Arguments const& args) {}


} // namespace jse