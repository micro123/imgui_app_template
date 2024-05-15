//
// Created by tang on 22-6-8.
//
#include "ImGuiUtils.h"

#include <cstdio>
#include <cstdint>
#include <string>
#include <cmath>
#include "utils/subprocess.hpp"

#include "text/IconsFontAwesome6.h" // from https://github.com/juliettef/IconFontCppHeaders
#include "imgui_internal.h"

ImGui::ColSwitchTemp::ColSwitchTemp(ImGuiCol_ toChange_, ImVec4 to_) : toChange(toChange_) {
    ImGuiStyle &style = ImGui::GetStyle();
    prevCol = style.Colors[toChange];
    style.Colors[toChange] = to_;
    bIsSet = true;
}

void ImGui::ColSwitchTemp::Reset() {
    if (bIsSet) {
        ImGui::GetStyle().Colors[toChange] = prevCol;
        bIsSet = false;
    }
}

ImGui::ColSwitchTemp::~ColSwitchTemp() {
    Reset();
}

void ImGui::SetupImGuiStyle(bool bStyleDark_, float alpha_) {
    ImGuiStyle &style = ImGui::GetStyle();

    // light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
    style.Alpha = 1.0f;
    style.FrameRounding = 3.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

    if (bStyleDark_) {
        for (int i = 0; i <= ImGuiCol_COUNT; i++) {
            ImVec4 &col = style.Colors[i];
            float H, S, V;
            ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

            if (S < 0.1f) {
                V = 1.0f - V;
            }
            ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
            if (col.w < 1.00f) {
                col.w *= alpha_;
            }
        }
    } else {
        for (int i = 0; i <= ImGuiCol_COUNT; i++) {
            ImVec4 &col = style.Colors[i];
            if (col.w < 1.00f) {
                col.x *= alpha_;
                col.y *= alpha_;
                col.z *= alpha_;
                col.w *= alpha_;
            }
        }
    }
}

bool ImGui::CheckBoxFont(const char *name_, bool *pB_, const char *pOn_, const char *pOff_) {
    if (*pB_) {
        ImGui::Text("%s %s", pOn_, name_);
    } else {
        ImGui::Text("%s %s", pOff_, name_);
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
        *pB_ = !*pB_;
        return true;
    }
    return false;
}

bool ImGui::CheckBoxTick(const char *name_, bool *pB_) {
    return CheckBoxFont(name_, pB_, ICON_FA_SQUARE_CHECK, ICON_FA_SQUARE);
}

bool ImGui::MenuItemCheckbox(const char *name_, bool *pB_) {
    char tempBuffer[255];
    ImGui::PushID(name_);
    if (*pB_) {
        snprintf(tempBuffer, sizeof(tempBuffer), ICON_FA_SQUARE_CHECK " %s", name_);
    } else {
        snprintf(tempBuffer, sizeof(tempBuffer), ICON_FA_SQUARE " %s", name_);
    }
    bool retval = ImGui::Selectable(tempBuffer, false, ImGuiSelectableFlags_DontClosePopups, ImVec2(0.0f, 0.0f));

    //bool retval = ImGui::MenuItem( tempBuffer );
    if (retval) {
        *pB_ = !*pB_;
    }
    ImGui::PopID();
    return retval;
}

bool ImGui::RadioSelectable(const char *label, int *v, int v_button, ImGuiSelectableFlags flags, const ImVec2 &size) {
    if (ImGui::Selectable(label, *v == v_button, flags, size)) {
        *v = v_button;
        return true;
    }
    return false;
}

bool ImGui::RadioCheckBoxTick(const char *name_, int *v, int v_button) {
    bool ticked = *v == v_button;
    if (ImGui::CheckBoxTick(name_, &ticked)) {
        *v = v_button;
        return true;
    }
    return false;
}

void ImGui::DummyWidget() {
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight() + ImGui::GetStyle().FramePadding.y * 2.0f));
}

void ImGui::DummyText() {
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight()));
}

void ImGui::ShowHelpMarker(const char *desc) {
    ImGui::TextDisabled(" " ICON_FA_CIRCLE_QUESTION " ");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(450.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ImGui::AddUnderLine(ImColor col_) {
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    min.y = max.y;
    ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
}

void ImGui::TextURL(const char *name_, const char *URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_) {
    if (1 == SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_Button]);
    ImGui::TextUnformatted(name_);
    ImGui::PopStyleColor();
    if (ImGui::IsItemHovered()) {
        if (ImGui::IsMouseClicked(0)) {
            subprocess::call ({"xdg-open", URL_});
        }
        AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
        ImGui::SetTooltip(ICON_FA_LINK " %s", URL_);
    } else {
        AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
    }
    if (1 == SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
}

void ImGui::MenuItemURL(const char *name_, const char *URL_) {
    std::string linkIconName = std::string(ICON_FA_LINK "\t" + std::string(name_));
    if (ImGui::MenuItem(linkIconName.c_str())) {
        subprocess::call ({"xdg-open", URL_});
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(ICON_FA_LINK " %s", URL_);
    }
}

bool ImGui::ToggleButton(const char *str_id, bool *v) {
    ImVec4* colors = ImGui::GetStyle().Colors;
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float height = ImGui::GetFrameHeight();
    float width = height * 1.55f;
    float radius = height * 0.50f;

    bool clicked = ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (clicked){
        *v = !*v;
    }
//    ImGuiContext& gg = *GImGui;
    if (ImGui::IsItemHovered())
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_ButtonHovered] : ImVec4(0.78f, 0.78f, 0.78f, 1.0f)), height * 0.5f);
    else
        draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(*v ? colors[ImGuiCol_Button] : ImVec4(0.85f, 0.85f, 0.85f, 1.0f)), height * 0.50f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + (*v ? 1 : 0) * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
    return clicked;
}

ImGui::FrameTimeHistogram::FrameTimeHistogram() {
    Clear();
}

void ImGui::FrameTimeHistogram::Clear() {
    timesTotal = 0.0f;
    countsTotal = 0.0f;
    memset(times, 0, sizeof(times));
    memset(counts, 0, sizeof(counts));
    memset(hitchTimes, 0, sizeof(hitchTimes));
    memset(hitchCounts, 0, sizeof(hitchCounts));
}

int ImGui::FrameTimeHistogram::GetBin(float time_) {
    int bin = (int) floor(time_ / dT);
    if (bin >= NUM) {
        bin = NUM - 1;
    }
    return bin;
}

void ImGui::FrameTimeHistogram::Update(float deltaT_) {
    if (deltaT_ < 0.0f) {
        assert(false);
        return;
    }
    int bin = GetBin(deltaT_);
    times[bin] += deltaT_;
    timesTotal += deltaT_;
    counts[bin] += 1.0f;
    countsTotal += 1.0f;

    float hitch = abs(lastdT - deltaT_);
    int deltaBin = GetBin(hitch);
    hitchTimes[deltaBin] += hitch;
    hitchCounts[deltaBin] += 1.0f;
    lastdT = deltaT_;
}

void ImGui::FrameTimeHistogram::PlotRefreshLines(float total_, float *pValues_) {
    ImDrawList *draw = ImGui::GetWindowDrawList();
    const ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 pad = style.FramePadding;
    ImVec2 min = ImGui::GetItemRectMin();
    min.x += pad.x;
    ImVec2 max = ImGui::GetItemRectMax();
    max.x -= pad.x;

    float xRefresh = (max.x - min.x) * refresh / (dT * NUM);

    float xCurr = xRefresh + min.x;
    while (xCurr < max.x) {
        float xP = ceil(xCurr); // use ceil to get integer coords or else lines look odd
        draw->AddLine(ImVec2(xP, min.y), ImVec2(xP, max.y), 0x50FFFFFF);
        xCurr += xRefresh;
    }

    if (pValues_) {
        // calc markers
        float currTotal = 0.0f;
        int mark = 0;
        for (int i = 0; i < NUM && mark < NUM_MARKERS; ++i) {
            currTotal += pValues_[i];
            if (total_ * markers[mark] < currTotal) {
                float xP = ceil((float) (i + 1) / (float) NUM * (max.x - min.x) +
                                min.x); // use ceil to get integer coords or else lines look odd
                draw->AddLine(ImVec2(xP, min.y), ImVec2(xP, max.y), 0xFFFF0000);
                ++mark;
            }
        }
    }
}

void ImGui::FrameTimeHistogram::CalcHistogramSize(int numShown_) {
    ImVec2 wRegion = ImGui::GetContentRegionMax();
    float heightGone = 7.0f * ImGui::GetTextLineHeightWithSpacing();
    wRegion.y -= heightGone;
    wRegion.y /= (float) numShown_;
    const ImGuiStyle &style = ImGui::GetStyle();
    ImVec2 pad = style.FramePadding;
    wRegion.x -= 2.0f * pad.x;
    size = wRegion;
}

void ImGui::FrameTimeHistogram::Draw(const char *name_, bool *pOpen_) {
    if (ImGui::Begin(name_, pOpen_)) {
        int numShown = 0;
        if (ImGui::CollapsingHeader("Time Histogram")) {
            ++numShown;
            ImGui::PlotHistogram("", times, NUM, 0, NULL, FLT_MAX, FLT_MAX, size);
            PlotRefreshLines(timesTotal, times);
        }
        if (ImGui::CollapsingHeader("Count Histogram")) {
            ++numShown;
            ImGui::PlotHistogram("", counts, NUM, 0, NULL, FLT_MAX, FLT_MAX, size);
            PlotRefreshLines(countsTotal, counts);
        }
        if (ImGui::CollapsingHeader("Hitch Time Histogram")) {
            ++numShown;
            ImGui::PlotHistogram("", hitchTimes, NUM, 0, NULL, FLT_MAX, FLT_MAX, size);
            PlotRefreshLines();
        }
        if (ImGui::CollapsingHeader("Hitch Count Histogram")) {
            ++numShown;
            ImGui::PlotHistogram("", hitchCounts, NUM, 0, NULL, FLT_MAX, FLT_MAX, size);
            PlotRefreshLines();
        }
        if (ImGui::Button("Clear")) {
            Clear();
        }
        CalcHistogramSize(numShown);
    }
    ImGui::End();
}
