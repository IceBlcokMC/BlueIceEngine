#include "api/ColorFormatAPI.h"
#include "endstone/color_format.h"
#include "utils/Convert.h"
#include "utils/Using.h"

namespace jse {


ClassDefine<void> ColorFormatAPI::builder =
    defineClass("ColorFormat")
        .property("Escape", []() { return ConvertToScriptX(endstone::ColorFormat::Escape); })
        .property("Black", []() { return ConvertToScriptX(endstone::ColorFormat::Black); })
        .property("DarkBlue", []() { return ConvertToScriptX(endstone::ColorFormat::DarkBlue); })
        .property("DarkGreen", []() { return ConvertToScriptX(endstone::ColorFormat::DarkGreen); })
        .property("DarkAqua", []() { return ConvertToScriptX(endstone::ColorFormat::DarkAqua); })
        .property("DarkRed", []() { return ConvertToScriptX(endstone::ColorFormat::DarkRed); })
        .property("DarkPurple", []() { return ConvertToScriptX(endstone::ColorFormat::DarkPurple); })
        .property("Gold", []() { return ConvertToScriptX(endstone::ColorFormat::Gold); })
        .property("Gray", []() { return ConvertToScriptX(endstone::ColorFormat::Gray); })
        .property("DarkGray", []() { return ConvertToScriptX(endstone::ColorFormat::DarkGray); })
        .property("Blue", []() { return ConvertToScriptX(endstone::ColorFormat::Blue); })
        .property("Green", []() { return ConvertToScriptX(endstone::ColorFormat::Green); })
        .property("Aqua", []() { return ConvertToScriptX(endstone::ColorFormat::Aqua); })
        .property("Red", []() { return ConvertToScriptX(endstone::ColorFormat::Red); })
        .property("LightPurple", []() { return ConvertToScriptX(endstone::ColorFormat::LightPurple); })
        .property("Yellow", []() { return ConvertToScriptX(endstone::ColorFormat::Yellow); })
        .property("White", []() { return ConvertToScriptX(endstone::ColorFormat::White); })
        .property("MinecoinGold", []() { return ConvertToScriptX(endstone::ColorFormat::MinecoinGold); })
        .property("MaterialQuartz", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialQuartz); })
        .property("MaterialIron", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialIron); })
        .property("MaterialNetherite", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialNetherite); })
        .property("MaterialRedstone", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialRedstone); })
        .property("MaterialCopper", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialCopper); })
        .property("MaterialGold", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialGold); })
        .property("MaterialEmerald", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialEmerald); })
        .property("MaterialDiamond", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialDiamond); })
        .property("MaterialLapis", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialLapis); })
        .property("MaterialAmethyst", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialAmethyst); })
        .property("MaterialResin", []() { return ConvertToScriptX(endstone::ColorFormat::MaterialResin); })
        .property("Obfuscated", []() { return ConvertToScriptX(endstone::ColorFormat::Obfuscated); })
        .property("Bold", []() { return ConvertToScriptX(endstone::ColorFormat::Bold); })
        .property("Italic", []() { return ConvertToScriptX(endstone::ColorFormat::Italic); })
        .property("Reset", []() { return ConvertToScriptX(endstone::ColorFormat::Reset); })
        .build();


} // namespace jse