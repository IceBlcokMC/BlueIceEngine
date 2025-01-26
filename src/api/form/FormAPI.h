#pragma once
#include "api/APIHelper.h"
#include "api/PlayerAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "utils/Convert.h"
#include "utils/Defines.h"
#include "utils/SafeTransfer.h"
#include "utils/Using.h"


namespace jse {

namespace detail {
template <typename Variant>
Local<Value> ConvertVariantToScriptX(Variant&& val) {
    return std::visit(
        [](auto&& arg) -> Local<Value> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return ConvertToScriptX(arg);
            } else if constexpr (std::is_same_v<T, endstone::Translatable>) {
                return TranslatableAPI::newTranslatableAPI(arg);
            } else {
                throw script::Exception("Unsupported variant type");
            }
        },
        std::forward<Variant>(val)
    );
}
} // namespace detail


template <typename T>
class FormAPI {
protected:
    T& mForm;

public:
    explicit FormAPI(T& form) : mForm(form) {}

    T& get() { return mForm; }

public:
    Local<Value> getTitle(Arguments const& /* args */) {
        try {
            return ConvertToScriptX(mForm.getTitle());
        }
        Catch;
    }

    Local<Value> setTitle(Arguments const& args) {
        CheckArgsCount(args, 1);
        try {
            if (args[0].isString()) {
                mForm.setTitle(ConvertFromScriptX<string>(args[0]));
            } else if (args[0].isObject() && IsInstanceOf<TranslatableAPI>(args[0])) {
                mForm.setTitle(GetScriptClass(TranslatableAPI, args[0])->get());
            } else {
                throw script::Exception("Invalid parameter type");
            }
            return args.thiz();
        }
        Catch;
    }

    Local<Value> setOnClose(Arguments const& args) {
        CheckArgsCount(args, 1);
        CheckArgType(args[0], ValueKind::kFunction);
        try {
            auto           engine = EngineScope::currentEngine();
            script::Global callback{args[0].asFunction()};

            auto ptr = SafeTransfer<Function>::make(engine, std::move(callback));

            mForm.setOnClose([ptr](endstone::Player* player) {
                if (ptr) {
                    EngineScope enter(ptr->mEngine);
                    try {
                        if (player) ptr->mGlobal.get().call({}, PlayerAPI::newPlayerAPI(player));
                        else ptr->mGlobal.get().call({});
                    }
                    CatchNotReturn;
                }
            });
            return args.thiz();
        }
        Catch;
    }

    // Local<Value> getOnClose(Arguments const& /* args */) {}
};


} // namespace jse