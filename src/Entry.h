#pragma once

#include "endstone/plugin/plugin.h"

namespace bie {


class Entry : public endstone::Plugin {
    struct Impl;
    std::unique_ptr<Impl> impl;

public:
    explicit Entry();

    void onLoad() override;

    void onEnable() override;

    void onDisable() override;

    endstone::PluginDescription const& getDescription() const override;

    [[nodiscard]] static Entry*& getInstance();
};


} // namespace bie