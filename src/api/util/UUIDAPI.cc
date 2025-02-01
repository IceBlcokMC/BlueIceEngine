#include "api/util/UUIDAPI.h"
#include "api/APIHelper.h"
#include "converter/Convert.h"
#include "utils/Defines.h"
#include "utils/Using.h"

namespace jse {


ClassDefine<UUIDAPI> UUIDAPI::builder = defineClass<UUIDAPI>("UUID")
                                            .constructor(&UUIDAPI::make)
                                            .instanceFunction("toString", &UUIDAPI::toString)
                                            .instanceFunction("forEach", &UUIDAPI::forEach)
                                            .instanceFunction("size", &UUIDAPI::size)
                                            .instanceFunction("isNil", &UUIDAPI::isNil)
                                            .instanceFunction("version", &UUIDAPI::version)
                                            .instanceFunction("swap", &UUIDAPI::swap)
                                            .instanceFunction("str", &UUIDAPI::str)
                                            .build();


UUIDAPI* UUIDAPI::make(const Arguments& args) {
    if (args.size() < 1) return nullptr;
    try {
        if (args[0].isString()) {
            // string constructor
            return new UUIDAPI(args.thiz(), fromString(args[0].asString().toString()));
        }
        if (args[0].isObject() && IsInstanceOf<UUIDAPI>(args[0])) {
            // copy constructor
            return new UUIDAPI(args.thiz(), GetScriptClass(UUIDAPI, args[0])->get());
        }
        throw script::Exception("invalid arguments");
    } catch (...) {
        return nullptr;
    }
}


Local<Value> UUIDAPI::toString(Arguments const& /* args */) { return ConvertToScript("<UUID>"); }

Local<Value> UUIDAPI::forEach(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kFunction);
    try {
        auto fn   = script::Global(args[0].asFunction());
        auto uuid = this->get();
        for (auto i : uuid) {
            fn.get().call(args.thiz(), ConvertToScript(i));
        }
        return Local<Value>();
    }
    Catch;
}

Local<Value> UUIDAPI::size(Arguments const& /* args */) { return ConvertToScript(this->get().size()); }

Local<Value> UUIDAPI::isNil(Arguments const& /* args */) { return ConvertToScript(this->get().isNil()); }

Local<Value> UUIDAPI::version(Arguments const& /* args */) { return ConvertToScript(this->get().version()); }

Local<Value> UUIDAPI::swap(Arguments const& args) {
    CheckArgsCount(args, 1);
    CheckArgType(args[0], ValueKind::kObject);
    try {
        if (!IsInstanceOf<UUIDAPI>(args[0])) {
            throw script::Exception("invalid arguments");
        }
        auto uuid = GetScriptClass(UUIDAPI, args[0])->get();
        this->get().swap(uuid);
        return Local<Value>();
    }
    Catch;
}

Local<Value> UUIDAPI::str(Arguments const& /* args */) { return ConvertToScript(this->get().str()); }


} // namespace jse