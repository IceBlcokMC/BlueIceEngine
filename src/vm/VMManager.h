#pragma once
#include "vm/VM.h"

#include <memory>


namespace bie {

class VMManager {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    VMManager();
    ~VMManager();

    VMManager(VMManager const&)            = delete;
    VMManager& operator=(VMManager const&) = delete;
    VMManager(VMManager&&)                 = delete;
    VMManager& operator=(VMManager&&)      = delete;

    bool initialize();

    bool shutdownVMs();

    void initUvThread();
    void shutdownUvThread();

    bool hasVM(VMID id) const;

    VM* createVM();

    void destroyVM(VMID id);

    void _performDestroy(VM* vm);
};


} // namespace bie