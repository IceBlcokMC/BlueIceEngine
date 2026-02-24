#pragma once

#include "endstone/plugin/plugin.h"

namespace bie {

class VMManager;

class Entry : public endstone::Plugin {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    explicit Entry();
    ~Entry() override;

    void onLoad() override;

    void onEnable() override;

    void onDisable() override;

    endstone::PluginDescription const& getDescription() const override;

    VMManager& getVMManager();

    [[nodiscard]] static Entry*& getInstance();
};


} // namespace bie