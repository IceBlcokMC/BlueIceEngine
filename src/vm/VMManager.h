#pragma once


namespace bie {


class VMManager {
public:
    VMManager();
    ~VMManager();

    VMManager(VMManager const&)            = delete;
    VMManager& operator=(VMManager const&) = delete;
    VMManager(VMManager&&)                 = delete;
    VMManager& operator=(VMManager&&)      = delete;
};


} // namespace bie