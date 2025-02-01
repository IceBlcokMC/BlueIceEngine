#include "api/ColorFormatAPI.h"
#include "converter/Convert.h"
#include "endstone/color_format.h"
#include "utils/Using.h"


namespace jse {


ClassDefine<void> ColorFormatAPI::builder =
    defineClass("ColorFormat")
        .property("Escape", []() { return ConvertToScript(endstone::ColorFormat::Escape); })
        .property("Black", []() { return ConvertToScript(endstone::ColorFormat::Black); })
        .property("DarkBlue", []() { return ConvertToScript(endstone::ColorFormat::DarkBlue); })
        .property("DarkGreen", []() { return ConvertToScript(endstone::ColorFormat::DarkGreen); })
        .property("DarkAqua", []() { return ConvertToScript(endstone::ColorFormat::DarkAqua); })
        .property("DarkRed", []() { return ConvertToScript(endstone::ColorFormat::DarkRed); })
        .property("DarkPurple", []() { return ConvertToScript(endstone::ColorFormat::DarkPurple); })
        .property("Gold", []() { return ConvertToScript(endstone::ColorFormat::Gold); })
        .property("Gray", []() { return ConvertToScript(endstone::ColorFormat::Gray); })
        .property("DarkGray", []() { return ConvertToScript(endstone::ColorFormat::DarkGray); })
        .property("Blue", []() { return ConvertToScript(endstone::ColorFormat::Blue); })
        .property("Green", []() { return ConvertToScript(endstone::ColorFormat::Green); })
        .property("Aqua", []() { return ConvertToScript(endstone::ColorFormat::Aqua); })
        .property("Red", []() { return ConvertToScript(endstone::ColorFormat::Red); })
        .property("LightPurple", []() { return ConvertToScript(endstone::ColorFormat::LightPurple); })
        .property("Yellow", []() { return ConvertToScript(endstone::ColorFormat::Yellow); })
        .property("White", []() { return ConvertToScript(endstone::ColorFormat::White); })
        .property("MinecoinGold", []() { return ConvertToScript(endstone::ColorFormat::MinecoinGold); })
        .property("MaterialQuartz", []() { return ConvertToScript(endstone::ColorFormat::MaterialQuartz); })
        .property("MaterialIron", []() { return ConvertToScript(endstone::ColorFormat::MaterialIron); })
        .property("MaterialNetherite", []() { return ConvertToScript(endstone::ColorFormat::MaterialNetherite); })
        .property("MaterialRedstone", []() { return ConvertToScript(endstone::ColorFormat::MaterialRedstone); })
        .property("MaterialCopper", []() { return ConvertToScript(endstone::ColorFormat::MaterialCopper); })
        .property("MaterialGold", []() { return ConvertToScript(endstone::ColorFormat::MaterialGold); })
        .property("MaterialEmerald", []() { return ConvertToScript(endstone::ColorFormat::MaterialEmerald); })
        .property("MaterialDiamond", []() { return ConvertToScript(endstone::ColorFormat::MaterialDiamond); })
        .property("MaterialLapis", []() { return ConvertToScript(endstone::ColorFormat::MaterialLapis); })
        .property("MaterialAmethyst", []() { return ConvertToScript(endstone::ColorFormat::MaterialAmethyst); })
        .property("MaterialResin", []() { return ConvertToScript(endstone::ColorFormat::MaterialResin); })
        .property("Obfuscated", []() { return ConvertToScript(endstone::ColorFormat::Obfuscated); })
        .property("Bold", []() { return ConvertToScript(endstone::ColorFormat::Bold); })
        .property("Italic", []() { return ConvertToScript(endstone::ColorFormat::Italic); })
        .property("Reset", []() { return ConvertToScript(endstone::ColorFormat::Reset); })
        .build();


} // namespace jse