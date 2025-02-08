#pragma once
#include "api/APIHelper.h"
#include "api/PlayerAPI.h"
#include "api/lang/TranslatableAPI.h"
#include "converter/Convert.h"
#include "endstone/message.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"

#include "converter/ProjectConverters.hpp"


namespace jse {


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
            return ConvertToScript(mForm.getTitle());
        }
        Catch;
    }

    Local<Value> setTitle(Arguments const& args) {
        CheckArgsCount(args, 1);
        CheckArgTypeOr(args[0], ValueKind::kString, ValueKind::kObject);
        try {
            mForm.setTitle(ConvertToCpp<endstone::Message>(args[0]));
            return args.thiz();
        }
        CatchAndThrow;
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
                        if (player) ptr->mGlobal.get().call({}, PlayerAPI::newInstance(player));
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