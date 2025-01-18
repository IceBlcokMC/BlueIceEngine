#pragma once
#include "utils/Defines.h"
#include "utils/Using.h"
#include <endstone/util/vector.h>

namespace jse {

class VectorAPI : public ScriptClass {
    endstone::Vector<float> mVector;

public:
    // C++ new
    explicit VectorAPI(endstone::Vector<float> vec)
    : ScriptClass(ConstructFromCpp<VectorAPI>{}),
      mVector(std::move(vec)) {}

    static Local<Object> newVectorAPI(endstone::Vector<float> vec) {
        return (new VectorAPI(std::move(vec)))->getScriptObject();
    }

    // Js new
    explicit VectorAPI(Local<Object> const& thiz, endstone::Vector<float> vec)
    : ScriptClass(thiz),
      mVector(std::move(vec)) {}

    static VectorAPI* make(Arguments const& args);

    endstone::Vector<float>& get() { return mVector; }

public: /* instanceProperty */
    Local<Value> GetterX();
    Local<Value> GetterY();
    Local<Value> GetterZ();

    void SetterX(Local<Value> const& value);
    void SetterY(Local<Value> const& value);
    void SetterZ(Local<Value> const& value);

public: /* Methods */
    METHODS(toString);
    METHODS(getX);
    METHODS(getY);
    METHODS(getZ);
    METHODS(setX);
    METHODS(setY);
    METHODS(setZ);

    METHODS(length);
    METHODS(lengthSquared);
    METHODS(distance);
    METHODS(distanceSquared);

public:
    static ClassDefine<VectorAPI> builder;
};


} // namespace jse