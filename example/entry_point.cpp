// #include <text/IconsFontAwesome6Brands.h>
// #include <app/entry_point.hpp>
// #include <utils/path_tools.hpp>
// #include "app/font_config.hpp"
// #include "imgui.h"
// #include "implot.h"
// #include "text/IconsFontAwesome6.h"

// std::pair<int, int> InitialWindowSize ()
// {
//     return {1400, 900};
// }

// const char *InitialWindowTitle ()
// {
//     return "ImGui App";
// }

// void ConfigImGui (ImGuiIO &io)
// {
//     io.IniFilename = nullptr;                              // disable state save
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
//     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
//                                                            // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable
//                                                            // Multi-Viewport / Platform Windows io.ConfigViewportsNoAutoMerge = true;
//                                                            // io.ConfigViewportsNoTaskBarIcon = true;
// }

// void ConfigStyle (ImGuiStyle &style)
// {
//     // Colors
//     ImVec4 *colors                         = style.Colors;
//     colors[ImGuiCol_Text]                  = ImVec4 (1.00f, 1.00f, 1.00f, 1.00f);
//     colors[ImGuiCol_TextDisabled]          = ImVec4 (0.63f, 0.63f, 0.63f, 1.00f);
//     colors[ImGuiCol_WindowBg]              = ImVec4 (0.15f, 0.22f, 0.30f, 1.00f);
//     colors[ImGuiCol_ChildBg]               = ImVec4 (0.32f, 0.33f, 0.34f, 1.00f);
//     colors[ImGuiCol_PopupBg]               = ImVec4 (0.16f, 0.24f, 0.23f, 1.00f);
//     colors[ImGuiCol_Border]                = ImVec4 (0.22f, 0.41f, 0.72f, 0.58f);
//     colors[ImGuiCol_BorderShadow]          = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
//     colors[ImGuiCol_FrameBg]               = ImVec4 (0.10f, 0.17f, 0.24f, 1.00f);
//     colors[ImGuiCol_FrameBgHovered]        = ImVec4 (0.14f, 0.17f, 0.11f, 1.00f);
//     colors[ImGuiCol_FrameBgActive]         = ImVec4 (0.18f, 0.18f, 0.04f, 1.00f);
//     colors[ImGuiCol_TitleBg]               = ImVec4 (0.00f, 0.00f, 0.00f, 1.00f);
//     colors[ImGuiCol_TitleBgActive]         = ImVec4 (0.05f, 0.15f, 0.29f, 1.00f);
//     colors[ImGuiCol_TitleBgCollapsed]      = ImVec4 (0.00f, 0.00f, 0.00f, 0.51f);
//     colors[ImGuiCol_MenuBarBg]             = ImVec4 (0.14f, 0.14f, 0.14f, 1.00f);
//     colors[ImGuiCol_ScrollbarBg]           = ImVec4 (0.18f, 0.18f, 0.18f, 1.00f);
//     colors[ImGuiCol_ScrollbarGrab]         = ImVec4 (0.50f, 0.50f, 0.50f, 1.00f);
//     colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4 (0.60f, 0.60f, 0.60f, 1.00f);
//     colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4 (0.60f, 0.60f, 0.60f, 1.00f);
//     colors[ImGuiCol_CheckMark]             = ImVec4 (0.93f, 0.93f, 0.93f, 1.00f);
//     colors[ImGuiCol_SliderGrab]            = ImVec4 (0.93f, 0.93f, 0.93f, 1.00f);
//     colors[ImGuiCol_SliderGrabActive]      = ImVec4 (0.42f, 0.57f, 0.09f, 1.00f);
//     colors[ImGuiCol_Button]                = ImVec4 (0.42f, 0.57f, 0.09f, 1.00f);
//     colors[ImGuiCol_ButtonHovered]         = ImVec4 (0.48f, 0.66f, 0.10f, 1.00f);
//     colors[ImGuiCol_ButtonActive]          = ImVec4 (0.15f, 0.20f, 0.03f, 1.00f);
//     colors[ImGuiCol_Header]                = ImVec4 (0.18f, 0.20f, 0.22f, 1.00f);
//     colors[ImGuiCol_HeaderHovered]         = ImVec4 (0.10f, 0.40f, 0.75f, 0.78f);
//     colors[ImGuiCol_HeaderActive]          = ImVec4 (0.26f, 0.59f, 0.98f, 1.00f);
//     colors[ImGuiCol_Separator]             = ImVec4 (0.43f, 0.43f, 0.50f, 0.50f);
//     colors[ImGuiCol_SeparatorHovered]      = ImVec4 (0.10f, 0.40f, 0.75f, 0.78f);
//     colors[ImGuiCol_SeparatorActive]       = ImVec4 (0.10f, 0.40f, 0.75f, 1.00f);
//     colors[ImGuiCol_ResizeGrip]            = ImVec4 (0.66f, 0.81f, 0.98f, 0.20f);
//     colors[ImGuiCol_ResizeGripHovered]     = ImVec4 (0.76f, 0.87f, 1.00f, 0.67f);
//     colors[ImGuiCol_ResizeGripActive]      = ImVec4 (0.83f, 0.91f, 1.00f, 0.95f);
//     colors[ImGuiCol_Tab]                   = ImVec4 (0.37f, 0.42f, 0.42f, 0.86f);
//     colors[ImGuiCol_TabHovered]            = ImVec4 (0.26f, 0.59f, 0.98f, 0.80f);
//     colors[ImGuiCol_TabActive]             = ImVec4 (0.20f, 0.41f, 0.68f, 1.00f);
//     colors[ImGuiCol_TabUnfocused]          = ImVec4 (0.07f, 0.10f, 0.15f, 0.97f);
//     colors[ImGuiCol_TabUnfocusedActive]    = ImVec4 (0.14f, 0.26f, 0.42f, 1.00f);
//     colors[ImGuiCol_DockingPreview]        = ImVec4 (0.26f, 0.59f, 0.98f, 0.70f);
//     colors[ImGuiCol_DockingEmptyBg]        = ImVec4 (0.20f, 0.20f, 0.20f, 1.00f);
//     colors[ImGuiCol_PlotLines]             = ImVec4 (0.61f, 0.61f, 0.61f, 1.00f);
//     colors[ImGuiCol_PlotLinesHovered]      = ImVec4 (1.00f, 0.43f, 0.35f, 1.00f);
//     colors[ImGuiCol_PlotHistogram]         = ImVec4 (0.90f, 0.70f, 0.00f, 1.00f);
//     colors[ImGuiCol_PlotHistogramHovered]  = ImVec4 (1.00f, 0.60f, 0.00f, 1.00f);
//     colors[ImGuiCol_TableHeaderBg]         = ImVec4 (0.19f, 0.19f, 0.20f, 1.00f);
//     colors[ImGuiCol_TableBorderStrong]     = ImVec4 (0.31f, 0.31f, 0.35f, 1.00f);
//     colors[ImGuiCol_TableBorderLight]      = ImVec4 (0.23f, 0.23f, 0.25f, 1.00f);
//     colors[ImGuiCol_TableRowBg]            = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
//     colors[ImGuiCol_TableRowBgAlt]         = ImVec4 (1.00f, 1.00f, 1.00f, 0.06f);
//     colors[ImGuiCol_TextSelectedBg]        = ImVec4 (0.26f, 0.59f, 0.98f, 0.35f);
//     colors[ImGuiCol_DragDropTarget]        = ImVec4 (0.78f, 1.00f, 0.59f, 0.90f);
//     colors[ImGuiCol_NavHighlight]          = ImVec4 (0.26f, 0.59f, 0.98f, 1.00f);
//     colors[ImGuiCol_NavWindowingHighlight] = ImVec4 (1.00f, 1.00f, 1.00f, 0.70f);
//     colors[ImGuiCol_NavWindowingDimBg]     = ImVec4 (0.80f, 0.80f, 0.80f, 0.20f);
//     colors[ImGuiCol_ModalWindowDimBg]      = ImVec4 (0.80f, 0.80f, 0.80f, 0.35f);

//     // Size
//     style.FrameRounding     = 4.f;
//     style.PopupRounding     = 4.f;
//     style.ChildRounding     = 4.f;
//     style.WindowRounding    = 4.f;
//     style.GrabRounding      = 8.f;
//     style.ScrollbarRounding = 8.f;
//     style.WindowPadding     = {8.f, 8.f};
//     style.FramePadding      = {6.f, 4.f};
//     style.ItemSpacing       = {6.f, 4.f};
// }

// std::vector<FontCfg> GetFontConfigs (ImGuiIO &io)
// {
//     std::vector<FontCfg> fonts;

//     // some ranges
//     static const ImWchar FA_RANGE[] = {ICON_MIN_FA, ICON_MAX_FA, 0, 0};  // FontAwesome
//     static const ImWchar SC_RANGE[] = {
//             32, 255, 8448, 8703, 19968, 40959, 65280, 65519, 0, 0,
//     };                                                          // Chinese Character & ASCII
//     static const ImWchar EMJ_RANGE[] = {126976, 131071, 0, 0};  // Emoji

//     FontCfg cfg;
//     cfg.name      = "Default Font";
//     cfg.size      = 14.0f;
//     cfg.bold      = 0;
//     cfg.sub_fonts = {
//             {FA_RANGE, "fonts/fa-solid-900.ttf"_AP},
//             {EMJ_RANGE, "fonts/seguiemj.ttf"_AP},
//             {SC_RANGE, "fonts/HarmonyOS_Sans_SC_Regular.ttf"_AP},
//     };

//     fonts.emplace_back (std::move (cfg));

//     return fonts;
// }

// void BeforeLoop () {}

// void AfterLoop () {}

// extern void SomeCrashFunction();

// void WindowContent ()
// {
//     using namespace ImGui;

//     static bool demo_imgui = false;
//     static bool demo_implot = false;

//     if (Begin ("First Window"))
//     {
//         TextUnformatted ("Hello World!");
//         TextUnformatted ("我是简体中文！");

//         SeparatorText("Demos");
//         Checkbox("ImGui Demo Window", &demo_imgui);
//         Checkbox("ImPlot Demo Window", &demo_implot);

//         SeparatorText("Testing");
//         if (Button("Make Me Crash")) {
//             SomeCrashFunction();
//         }
//     }
//     End ();

//     if (demo_imgui)
//         ImGui::ShowDemoWindow(&demo_imgui);
//     if (demo_implot)
//         ImPlot::ShowDemoWindow(&demo_implot);
// }

#include <text/IconsFontAwesome6.h>
#include <app/application.hpp>
#include <memory>
#include <imgui.h>
#include <implot.h>
#include <utils/path_tools.hpp>

extern void SomeCrashFunction();

class MyApp final : public Application
{
public:
    MyApp () : Application ("Hello World", 1280, 720, AppWindowFlag_Maximum) {}

protected:
    void Update (double delta, double total) override
    {
        using namespace ImGui;

        static bool demo_imgui  = false;
        static bool demo_implot = false;

        if (Begin ("First Window"))
        {
            Text("Delta: %.3f", delta);
            Text("Total: %.3f", total);

            SeparatorText("Simple Widgets");

            TextUnformatted ("Hello World!");
            TextUnformatted ("我是简体中文！");

            SeparatorText ("Demos");
            Checkbox ("ImGui Demo Window", &demo_imgui);
            Checkbox ("ImPlot Demo Window", &demo_implot);

            SeparatorText ("Testing");
            if (Button ("Make Me Crash"))
            {
                SomeCrashFunction ();
            }
            if (Button("Quit"))
            {
                App->Exit(1);
            }
        }
        End ();

        if (demo_imgui)
            ImGui::ShowDemoWindow (&demo_imgui);
        if (demo_implot)
            ImPlot::ShowDemoWindow (&demo_implot);
    }

    std::vector<FontCfg> GetFontConfigs (ImGuiIO &io) override
    {
        std::vector<FontCfg> fonts;

        // some ranges
        static const ImWchar FA_RANGE[] = {ICON_MIN_FA, ICON_MAX_FA, 0, 0};  // FontAwesome
        static const ImWchar SC_RANGE[] = {
                32, 255, 8448, 8703, 19968, 40959, 65280, 65519, 0, 0,
        };                                                          // Chinese Character & ASCII
        static const ImWchar EMJ_RANGE[] = {126976, 131071, 0, 0};  // Emoji

        FontCfg cfg;
        cfg.name      = "Default Font";
        cfg.size      = 14.0f;
        cfg.bold      = 0;
        cfg.sub_fonts = {
                {FA_RANGE, "fonts/fa-solid-900.ttf"_AP},
                {EMJ_RANGE, "fonts/seguiemj.ttf"_AP},
                {SC_RANGE, "fonts/HarmonyOS_Sans_SC_Regular.ttf"_AP},
        };

        fonts.emplace_back (std::move (cfg));

        return fonts;
    }
};

UniqueApp CreateApplication ()
{
    return std::make_unique<MyApp> ();
}
