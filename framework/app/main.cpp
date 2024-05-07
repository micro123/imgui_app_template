#include "GLFW/glfw3.h"
#include "app/entry_point.hpp"
#include "app/font_config.hpp"
#include "imgui.h"
#include "implot.h"
#include "misc/freetype/imgui_freetype.h"
#include "utils/backward.hpp"
#include <core/log.hpp>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <cstdio>
#include <cstdlib>
#include <vector>

static void glfw_error_handler(int code, const char *msg) {
    L_ERROR("GLFW Error %d: %s\n", code, msg);
}

static void LoadFonts(const std::vector<FontCfg> &cfgs, ImGuiIO &io) {
    if (!cfgs.empty()) {
        for (auto &x: cfgs) {
            L_INFO("Loading Font: %s", x.name.c_str());
            bool first = true;
            for (auto &y: x.sub_fonts) {
                L_INFO("  Add Font From %s", y.path.c_str());
                ImFontConfig font_cfg;
                strncpy(font_cfg.Name, x.name.c_str(), std::size(font_cfg.Name) - 1);
                font_cfg.MergeMode = !first;
                font_cfg.PixelSnapH = true;
                font_cfg.FontBuilderFlags = (ImGuiFreeTypeBuilderFlags_Bitmap | ImGuiFreeTypeBuilderFlags_LoadColor);
                if (x.bold)
                    font_cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
                auto font = io.Fonts->AddFontFromFileTTF(y.path.c_str(), x.size, &font_cfg, y.ranges);
                LC_FATAL(!font, "Font Add Failed!");
                first = false;
            }
        }
    } else {
        L_INFO("Load default fonts.");
        io.Fonts->AddFontDefault();
    }
    bool ok = io.Fonts->Build();
    LC_FATAL(!ok, "Font Build Failed!");
}

int main(int argc, const char** argv) {
    // crash handlers
    backward::SignalHandling sh;

    L_INFO("Starting...");

    glfwSetErrorCallback(&glfw_error_handler);
    glfwInit();

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    auto const size = InitialWindowSize();
    auto wnd = glfwCreateWindow(size.first, size.second, InitialWindowTitle(), nullptr, nullptr);
    LC_FATAL(!wnd, "Window creation error!");

    glfwMakeContextCurrent(wnd);
    glfwSwapInterval(1); // enable v-sync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    auto &io = ImGui::GetIO();

    // User: config imgui flags
    L_DEBUG("Configuring ImGui");
    ConfigImGui(io);

    // User: Style
    L_DEBUG("Configuring ImGui Styles");
    ConfigStyle(ImGui::GetStyle());

    // backend inits
    ImGui_ImplGlfw_InitForOpenGL(wnd, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    L_INFO("ImGui Init Done!");

    // User: Fonts
    L_INFO("Preparing Load Fonts.");
    auto fonts = GetFontConfigs(io);
    LoadFonts(fonts, io);

    // User: Post Loop
    L_INFO("User Init Process...");
    BeforeLoop();

    L_INFO("Enter MainLoop.");
    // loop
    while(!glfwWindowShouldClose(wnd)) {
        glfwPollEvents(); // process glfw events

        // new frame
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // User Content
        WindowContent();

        // render
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(wnd, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // multi viewports
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(wnd);
    }
    L_INFO("Exiting MainLoop.");

    // User: End Loop
    L_INFO("User Cleanup Process.");
    AfterLoop();

    // cleanup
    L_INFO("Cleanup.");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(wnd);
    glfwTerminate();
    return 0;
}
