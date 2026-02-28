#pragma once
#include "vm/VM.h"

namespace bie {


struct VMData {
    VMID const                   id;
    std::string                  fileName;
    v8kit::Global<v8kit::Object> pluginInst{};

    explicit VMData(VMID id) : id(id) {}
    ~VMData() { pluginInst.reset(); }
};


} // namespace bie