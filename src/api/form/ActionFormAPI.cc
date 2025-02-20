#include "ActionFormAPI.h"
#include "api/APIHelper.h"
#include "api/PlayerAPI.h"
#include "api/form/FormAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "converter/Convert.h"
#include "endstone/message.h"
#include "endstone/player.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"
#include <optional>
#include <variant>


namespace jse {

ClassDefine<ActionFormAPI> ActionFormAPI::builder = defineClass<ActionFormAPI>("ActionForm")
                                                        .constructor(&ActionFormAPI::make)
                                                        .instanceFunction("toString", &ActionFormAPI::toString)
                                                        .instanceFunction("getContent", &ActionFormAPI::getContent)
                                                        .instanceFunction("setContent", &ActionFormAPI::setContent)
                                                        .instanceFunction("addButton", &ActionFormAPI::addButton)
                                                        // .instanceFunction("getButtons", &ActionFormAPI::getButtons)
                                                        // .instanceFunction("setButtons", &ActionFormAPI::setButtons)
                                                        // .instanceFunction("getOnSubmit", &ActionFormAPI::getOnSubmit)
                                                        .instanceFunction("setOnSubmit", &ActionFormAPI::setOnSubmit)

                                                        // ActionFormAPI extends FormAPI
                                                        .instanceFunction("getTitle", &ActionFormAPI::getTitle)
                                                        .instanceFunction("setTitle", &ActionFormAPI::setTitle)
                                                        .instanceFunction("setOnClose", &ActionFormAPI::setOnClose)
                                                        .build();

ActionFormAPI* ActionFormAPI::make(Arguments const& args) { return new ActionFormAPI(args.thiz()); }


Local<Value> ActionFormAPI::toString(Arguments const& /* args */) {
    try {
        return ConvertToScript("<ActionForm>");
    }
    Catch;
}

Local<Value> ActionFormAPI::getContent(Arguments const& /* args */) {
    try {
        return ConvertToScript(get().getContent());
    }
    Catch;
}

Local<Value> ActionFormAPI::setContent(Arguments const& args) {
    CheckArgsCount(args, 1);
    try {
        mActionForm.setContent(ConvertToCpp<endstone::Message>(args[0]));
        return args.thiz();
    }
    Catch;
}


Local<Value> ActionFormAPI::addButton(Arguments const& args) {
    CheckArgsCount(args, 1); // 最少 1 个参数
    try {
        auto val = ConvertToCpp<endstone::Message>(args[0]);
        if (args.size() == 1) {
            mActionForm.addButton(val);
        } else if (args.size() == 2) {
            mActionForm.addButton(val, ConvertToCpp<string>(args[1]));
        } else if (args.size() == 3) {
            auto ptr = SafeTransfer<Function>::make(EngineScope::currentEngine(), script::Global{args[2].asFunction()});
            mActionForm.addButton(val, ConvertToCpp<string>(args[1]), [ptr](endstone::Player* player) {
                if (ptr) {
                    EngineScope enter{ptr->mEngine};
                    try {
                        ptr->mGlobal.get().call({}, player ? PlayerAPI::newInstance(player) : Local<Value>{});
                    }
                    CatchNotReturn;
                }
            });
        }
        return args.thiz();
    }
    Catch;
}

// Local<Value> ActionFormAPI::getButtons(Arguments const& args) {}
// Local<Value> ActionFormAPI::setButtons(Arguments const& args) {}
// Local<Value> ActionFormAPI::getOnSubmit(Arguments const& args) {}

Local<Value> ActionFormAPI::setOnSubmit(Arguments const& args) {
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


} // namespace jse