#pragma once

#include <cstdint>
#include "imgui.h"

namespace ImGui {
    class IMGUI_API ColSwitchTemp {
        ImGuiCol_ toChange;
        ImVec4 prevCol;
        bool bIsSet;
    public:
        ColSwitchTemp(ImGuiCol_ toChange_, ImVec4 to_);

        void Reset();

        ~ColSwitchTemp();

    };

    void IMGUI_API SetupImGuiStyle(bool bStyleDark_, float alpha_);

    bool IMGUI_API CheckBoxFont(const char *name_, bool *pB_, const char *pOn_ = "[X]", const char *pOff_ = "[ ]");

    bool IMGUI_API CheckBoxTick(const char *name_, bool *pB_);

    bool IMGUI_API MenuItemCheckbox(const char *name_, bool *pB_);

    bool IMGUI_API RadioSelectable(const char *label, int *v, int v_button, ImGuiSelectableFlags flags = 0,
                                   const ImVec2 &size = ImVec2(0, 0));

    bool IMGUI_API RadioCheckBoxTick(const char *name_, int *v, int v_button);

    void IMGUI_API DummyWidget();

    void IMGUI_API DummyText();

    void IMGUI_API ShowHelpMarker(const char *desc);

    void IMGUI_API AddUnderLine(ImColor col_);

    // hyperlink urls
    void IMGUI_API TextURL(const char *name_, const char *URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_);

    // menu item urls
    void IMGUI_API MenuItemURL(const char *name_, const char *URL_);

    struct IMGUI_API FrameTimeHistogram {
        // configuration params - modify these at will
        static const int NUM = 101; //last value is from T-1 to inf.

        float dT = 0.001f;    // in seconds, default 1ms
        float refresh = 1.0f / 60.0f;// set this to your target refresh rate

        static const int NUM_MARKERS = 2;
        float markers[NUM_MARKERS] = {0.99f, 0.999f};

        // data
        ImVec2 size = ImVec2(3.0f * NUM, 40.0f);
        float lastdT = 0.0f;
        float timesTotal;
        float countsTotal;
        float times[NUM];
        float counts[NUM];
        float hitchTimes[NUM];
        float hitchCounts[NUM];

        FrameTimeHistogram();

        void Clear();

        int GetBin(float time_);

        void Update(float deltaT_);

        void PlotRefreshLines(float total_ = 0.0f, float *pValues_ = nullptr);

        void CalcHistogramSize(int numShown_);

        void Draw(const char *name_, bool *pOpen_ = nullptr);
    };

    bool IMGUI_API ToggleButton(const char* str_id, bool* v);
};
