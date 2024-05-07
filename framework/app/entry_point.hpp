#ifndef ENTRY_POINT_HPP
#define ENTRY_POINT_HPP

#include <imgui.h>
#include <utility>
#include <vector>

#include <app/font_config.hpp>

// basic info for window
std::pair<int,int> InitialWindowSize();
const char* InitialWindowTitle();

// imgui config
void ConfigImGui(ImGuiIO &io);
void ConfigStyle(ImGuiStyle &style);
std::vector<FontCfg> GetFontConfigs(ImGuiIO &io);

// custom content
void BeforeLoop(); // called before enter mainloop
void WindowContent(); // call every frame
void AfterLoop(); // called after mainloop

#endif /* ENTRY_POINT_HPP */
