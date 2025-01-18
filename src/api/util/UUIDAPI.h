#pragma once
#include "endstone/util/uuid.h"
#include "utils/Defines.h"
#include "utils/Using.h"
#include <algorithm>


namespace jse {


class UUIDAPI : public ScriptClass {
    endstone::UUID mUUID;

    static endstone::UUID fromString(string const& str) {
        if (str.length() != 36) {
            throw std::invalid_argument("Invalid UUID string length");
        }
        endstone::UUID uuid;
        size_t         index = 0;
        for (size_t i = 0; i < 16; ++i) {
            uint8_t byte = 0;
            if (i == 4 || i == 6 || i == 8 || i == 10) {
                if (str[index] != '-') {
                    throw std::invalid_argument("Invalid UUID string format");
                }
                index++;
            }
            byte         |= static_cast<uint8_t>(hexCharToInt(str[index++]) << 4);
            byte         |= static_cast<uint8_t>(hexCharToInt(str[index++]));
            uuid.data[i]  = byte;
        }
        return uuid;
    }

    static uint8_t hexCharToInt(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        } else if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        throw std::invalid_argument("Invalid hex character");
    }

public:
    // C++ new
    explicit UUIDAPI(endstone::UUID uuid) : ScriptClass(ConstructFromCpp<UUIDAPI>{}), mUUID(std::move(uuid)) {}

    static Local<Object> newUUIDAPI(endstone::UUID uuid) { return (new UUIDAPI(std::move(uuid)))->getScriptObject(); }

    static Local<Object> newUUIDAPI(string const& uuid) { return (new UUIDAPI(fromString(uuid)))->getScriptObject(); }

    // Js new
    explicit UUIDAPI(Local<Object> const& thiz, endstone::UUID uuid) : ScriptClass(thiz), mUUID(std::move(uuid)) {}

    static UUIDAPI* make(Arguments const& args);

    endstone::UUID& get() { return mUUID; }

public:
    METHODS(toString);

    METHODS(forEach); // c++ iterate

    METHODS(size);

    METHODS(isNil);

    METHODS(version);

    METHODS(swap);

    METHODS(str);


public:
    static ClassDefine<UUIDAPI> builder;
};


} // namespace jse