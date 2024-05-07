#ifndef FONT_CONFIG_HPP
#define FONT_CONFIG_HPP

#include <vector>
#include <string>

#include "imgui.h"

struct FontCfg {
    struct SubFont {
        // range of this font
        const ImWchar *ranges;
        std::string    path;
    };

    std::string          name;
    std::vector<SubFont> sub_fonts;
    float                size;
    int                  bold;
};

#endif /* FONT_CONFIG_HPP */
