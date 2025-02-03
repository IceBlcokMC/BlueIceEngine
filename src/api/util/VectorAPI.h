#pragma once
#include "api/APIHelper.h"
#include "utils/ResourceSafety.h"
#include "utils/Using.h"
#include <endstone/util/vector.h>
#include <memory>

namespace jse {

class VectorAPI : public ScriptClass {
    SafePointerHolder<endstone::Vector<float>> mData;

public:
    explicit VectorAPI(endstone::Vector<float>* vec) : ScriptClass(ConstructFromCpp<VectorAPI>{}), mData(vec) {}

    explicit VectorAPI(Local<Object> const& thiz, std::unique_ptr<endstone::Vector<float>> vec)
    : ScriptClass(thiz),
      mData(std::move(vec)) {}

public:
    static Local<Object> newInstance(endstone::Vector<float>* vec) { return (new VectorAPI(vec))->getScriptObject(); }

    static VectorAPI* make(Arguments const& args);

    endstone::Vector<float>* get() { return mData.get(); }

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