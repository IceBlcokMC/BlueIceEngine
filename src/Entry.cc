#include "Entry.h"

#include "endstone/plugin/plugin_description.h"


namespace bie {

class PluginDescriptionImpl : public endstone::detail::PluginDescriptionBuilder {
public:
    PluginDescriptionImpl() {
        description  = "A JavaScript runtime for endstone";
        contributors = {"BlueIceMc/BlueIceEngine all contributors"};
        website      = "https://github.com/IceBlcokMC/BlueIceEngine";
    }
};

struct Entry::Impl {
    endstone::PluginDescription description = PluginDescriptionImpl{}.build("blue_ice_engine", "");
};

Entry::Entry() : impl(std::make_unique<Impl>()) {}

void Entry::onLoad() {}

void Entry::onEnable() {}

void Entry::onDisable() {}

endstone::PluginDescription const& Entry::getDescription() const { return impl->description; }


Entry*& Entry::getInstance() {
    static Entry* instance = nullptr; // init_endstone_plugin will set this
    return instance;
}

} // namespace bie

extern "C" [[maybe_unused]] ENDSTONE_EXPORT endstone::Plugin* init_endstone_plugin() {
    bie::Entry::getInstance() = new bie::Entry();
    return bie::Entry::getInstance();
}