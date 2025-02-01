#pragma once
#include "manager/EngineData.h"
#include "utils/Using.h"


namespace jse {

[[nodiscard]] bool IsFloat(Local<Value> const& num);

[[nodiscard]] string ToString(ValueKind const& kind);
[[nodiscard]] string ToString(Local<Value> const& value);
void                 ToString(Local<Value> const& value, std::ostringstream& oss);
void                 ToString(Local<Array> const& value, std::ostringstream& oss);
void                 ToString(Local<Object> const& value, std::ostringstream& oss);

template <typename T>
[[nodiscard]] inline bool IsInstanceOf(Local<Value> const& value) {
    return EngineScope::currentEngine()->isInstanceOf<T>(value);
}

void PrintException(script::Exception const& e, string const& func, string const& plugin, string const& api);
void PrintException(std::exception const& err, string const& func, string const& plugin, string const& api);
void PrintException(string const& msg, string const& func, string const& plugin, string const& api);

#define PrintScriptError(msg_or_exception)                                                                             \
    PrintException(msg_or_exception, __func__, ENGINE_DATA()->mFileName, __FUNCTION__);

/* 预定义异常 */
#define ERR_WRONG_ARG_TYPE   "Wrong argument type, please check the documentation" /* 参数类型错误，请查阅文档 */
#define ERR_TOO_FEW_ARGS     "Too few arguments"                                   /* 参数数量不足 */
#define ERR_WRONG_ARGS_COUNT "Wrong arguments count"                               /* 参数数量错误 */


/* 参数检查 */
#define _CHECK_IMPL(EXPRESSION, ERR, RET_CODE)                                                                         \
    if (EXPRESSION) {                                                                                                  \
        PrintScriptError(ERR);                                                                                         \
        RET_CODE;                                                                                                      \
    }
#define _ONLY_CHECK_IMPL(EXPRESSION, ERR) _CHECK_IMPL(EXPRESSION, ERR, 0) // 仅检查，不返回值(填0避免编译器警告)

/* 检查参数数量(错误返回 undefined) */
#define CheckArgsCount(args, count) _CHECK_IMPL(args.size() < count, ERR_TOO_FEW_ARGS, Local<Value>())

/* 检查参数类型(错误返回 undefined) */
#define CheckArgType(arg, type) _CHECK_IMPL(arg.getKind() != type, ERR_WRONG_ARG_TYPE, Local<Value>())

/* 检查参数是否是类型1或2(错误返回 undefined) */
#define CheckArgTypeOr(arg, type1, type2)                                                                              \
    _CHECK_IMPL(arg.getKind() != type1 && arg.getKind() != type2, ERR_WRONG_ARG_TYPE, Local<Value>())

/* 仅检查参数数量 */
#define CheckArgsCountVoid(args, count) _ONLY_CHECK_IMPL(args.size() < count, ERR_TOO_FEW_ARGS)

/* 仅检查参数类型 */
#define CheckArgTypeVoid(arg, type) _ONLY_CHECK_IMPL(arg.getKind() != type, ERR_WRONG_ARG_TYPE)

/* 仅检查参数是否是类型1或2 */
#define CheckArgTypeOrVoid(arg, type1, type2)                                                                          \
    _ONLY_CHECK_IMPL(arg.getKind() != type1 && arg.getKind() != type2, ERR_WRONG_ARG_TYPE)


/* 异常处理 */
#define _CATCH_IMPL(RET_CODE)                                                                                          \
    catch (script::Exception const& e) {                                                                               \
        PrintScriptError(e) RET_CODE;                                                                                  \
    }                                                                                                                  \
    catch (std::exception const& e) {                                                                                  \
        PrintScriptError(e) RET_CODE;                                                                                  \
    }                                                                                                                  \
    catch (...) {                                                                                                      \
        PrintScriptError("Unknown exception") RET_CODE;                                                                \
    }

/* 仅捕获异常 */
#define CatchNotReturn _CATCH_IMPL(0) // 填0，防止编译器警告

/* 捕获异常并返回自定义类型 */
#define CatchAndReturn(RET_VALUE) _CATCH_IMPL(return RET_VALUE)

/* 捕获异常并返回 undefined */
#define Catch CatchAndReturn(Local<Value>())


/* 捕获异常并抛回脚本层 */
#define CatchAndThrow                                                                                                  \
    catch (script::Exception const& e) {                                                                               \
        throw e;                                                                                                       \
    }                                                                                                                  \
    catch (std::exception const& e) {                                                                                  \
        throw script::Exception(e.what());                                                                             \
    }                                                                                                                  \
    catch (...) {                                                                                                      \
        throw script::Exception("Unknown exception in " __FUNCTION__);                                                 \
    }


/* 脚本辅助宏 */
#define METHODS(FUNC) Local<Value> FUNC(Arguments const& args)

#define GetScriptClass(CLASS, VAL) EngineScope::currentEngine()->getNativeInstance<CLASS>(VAL)

} // namespace jse