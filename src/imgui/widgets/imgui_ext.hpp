#ifndef IMGUI_EXT_HPP
#define IMGUI_EXT_HPP

#include <initializer_list>

#include <imgui.h>

namespace ImGui {
    IMGUI_API void TextToolTipMaker(const char *text);
    IMGUI_API void TextHelperSign(const char *fmt, ...);
    IMGUI_API void TextInformationSign(const char *fmt, ...);
    IMGUI_API bool TimedBeginToolTip();
    IMGUI_API void TimedEndToolTip();
    IMGUI_API void CenterText(const char *text, float area_width);
    IMGUI_API void ContentCenter(const char *text);
    IMGUI_API void CenterWindow(ImVec2 size);
    IMGUI_API void CenterWindowPercent(float of_full_screen);
    IMGUI_API int  BottomButtons(const std::initializer_list<const char*> &buttons);
}

#endif /* IMGUI_EXT_HPP */
