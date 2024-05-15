#include "imgui_ext.hpp"
#include "core/assert.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "text/IconsFontAwesome6.h"

namespace ImGui {
    void TextToolTipMaker(const char *text) {
        static double last_hover_time = 0;
        static ImGuiID last_hover_id = -1;

        double currTime = ImGui::GetTime();
        ImGuiID hoveredID = ImGui::GetHoveredID();

        // 250ms hover
        if (IsItemHovered() && (currTime - last_hover_time) >= 0.25 && hoveredID == last_hover_id) {
            BeginTooltip();
            TextUnformatted(text);
            EndTooltip();
        }

        if (hoveredID != last_hover_id) {
            last_hover_time = currTime;
        }
        last_hover_id = hoveredID;
    }

    void CenterText(const char *text, float area_width) {
        const auto w = CalcTextSize(text).x;
        if (area_width > w)
        {
            SetCursorPosX((area_width - w) / 2);
        }
        TextUnformatted(text);
    }

    static void TextSignV(const char *sign, const char *fmt, va_list args) {
        SameLine(0, GetFontSize()/4.f);
        TextUnformatted(sign);
        if (IsItemHovered()) {
            SetTooltipV(fmt, args);
        }
    }

    void TextHelperSign(const char *fmt, ...)
    {
        va_list args;
        va_start (args, fmt);
        TextSignV (ICON_FA_CIRCLE_QUESTION, fmt, args);
        va_end (args);
    }

    void TextInformationSign(const char *fmt, ...)
    {
        va_list args;
        va_start (args, fmt);
        TextSignV (ICON_FA_CIRCLE_INFO, fmt, args);
        va_end (args);
    }

    bool TimedBeginToolTip() {
        static double last_hover_time = 0;
        static ImGuiID last_hover_id = -1;

        double currTime = ImGui::GetTime();
        ImGuiID hoveredID = ImGui::GetHoveredID();

        // 500ms hover
        if (IsItemHovered() && (currTime - last_hover_time) >= 1.0 && hoveredID == last_hover_id) {
            BeginTooltip();
            return true;
        }

        if (hoveredID != last_hover_id) {
            last_hover_time = currTime;
        }
        last_hover_id = hoveredID;
        return false;
    }

    void TimedEndToolTip() {
        EndTooltip();
    }

    void CenterWindow (ImVec2 size) {
        auto vp = GetMainViewport();
        auto pos = (vp->Size - size) / 2.f;
        SetNextWindowPos (pos, ImGuiCond_Always);
        SetNextWindowSize (size);
    }

    void CenterWindowPercent (float of_full_screen) {
        CenterWindow (GetMainViewport()->Size * of_full_screen);
    }

    int BottomButtons(const std::initializer_list<const char*> &buttons)
    {
        PI_ASSERT(!buttons.size());

        auto const [maxX,maxY] = GetCurrentWindow()->Size;
        auto const [curX,curY] = GetCursorPos();

        int clicked = -1;

        // 1. 计算字符长度
        float total_length = 0.f;
        for (auto const &text: buttons) {
            total_length += (CalcTextSize (text).x);
        }
        total_length += (buttons.size() - 1) * GetFontSize();

        // 2. 算出光标位置
        SetCursorPosX ((maxX - total_length) / 2.f);
        SetCursorPosY (curY + (maxY - curY - GetTextLineHeightWithSpacing() - GetStyle().ItemSpacing.y*2.f) / 2.f);

        // 3. 绘制按钮
        int i = 0;
        for (const auto &text: buttons) {
            if (Button (text))
                clicked = i;
            SameLine ();
            i++;
        }
        return clicked;
    }

    void ContentCenter (const char *text) {
        const auto w = CalcTextSize(text).x;
        const auto content_size = GetContentRegionAvail();
        if (content_size.x > w && content_size.y > GetTextLineHeightWithSpacing())
        {
            SetCursorPosY((content_size.y - GetTextLineHeightWithSpacing()) / 2.f);
            SetCursorPosX((content_size.x - w) / 2);
        }
        TextUnformatted(text);
    }
}
