#include "MessageFormAPI.h"
#include "api/APIHelper.h"
#include "api/PlayerAPI.h"
#include "api/form/FormAPI.h"
#include "endstone/endstone.h"
#include "utils/Convert.h"
#include "utils/SafeTransfer.h"
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
        return ConvertToScriptX("<MessageForm>");
    }
    Catch;
}

Local<Value> MessageFormAPI::getContent(Arguments const& /* args */) {
    try {
        return detail::ConvertVariantToScriptX(get().getContent());
    }
    Catch;
}

Local<Value> MessageFormAPI::setContent(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        if (args[0].isString()) {
            mMessageForm.setContent(ConvertFromScriptX<string>(args[0]));
        } else if (args[0].isObject() && IsInstanceOf<TranslatableAPI>(args[0])) {
            mMessageForm.setContent(GetScriptClass(TranslatableAPI, args[0])->get());
        } else {
            throw script::Exception("Invalid parameter type");
        }
        return args.thiz();
    }
    Catch;
}

Local<Value> MessageFormAPI::getButton1(Arguments const& /* args */) {
    try {
        return detail::ConvertVariantToScriptX(get().getButton1());
    }
    Catch;
}

Local<Value> MessageFormAPI::setButton1(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        if (args[0].isString()) {
            mMessageForm.setButton1(ConvertFromScriptX<string>(args[0]));
        } else if (args[0].isObject() && IsInstanceOf<TranslatableAPI>(args[0])) {
            mMessageForm.setButton1(GetScriptClass(TranslatableAPI, args[0])->get());
        } else {
            throw script::Exception("Invalid parameter type");
        }
        return args.thiz();
    }
    Catch;
}

Local<Value> MessageFormAPI::getButton2(Arguments const& /* args */) {
    try {
        return detail::ConvertVariantToScriptX(get().getButton2());
    }
    Catch;
}

Local<Value> MessageFormAPI::setButton2(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        if (args[0].isString()) {
            mMessageForm.setButton2(ConvertFromScriptX<string>(args[0]));
        } else if (args[0].isObject() && IsInstanceOf<TranslatableAPI>(args[0])) {
            mMessageForm.setButton2(GetScriptClass(TranslatableAPI, args[0])->get());
        } else {
            throw script::Exception("Invalid parameter type");
        }
        return args.thiz();
    }
    Catch;
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
                    if (player) ptr->mGlobal.get().call({}, PlayerAPI::newInstance(player), ConvertToScriptX(idk));
                    else ptr->mGlobal.get().call({}, Local<Value>{}, ConvertToScriptX(idk));
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