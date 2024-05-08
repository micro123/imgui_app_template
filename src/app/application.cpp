#include "application.hpp"
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <implot.h>
#include <misc/freetype/imgui_freetype.h>
#include "core/log.hpp"

namespace
{
    // internal state
    volatile bool exit_reqested = false;
    volatile int  exit_code     = 0;
    Application  *instance      = nullptr;

    // init state
    const char *glsl_version = "#version 130";
    GLFWwindow *window;

    // clear color
    ImVec4 clear_color{0,0,0,1};
}  // namespace

static void glfw_error_handler (int code, const char *msg)
{
    L_ERROR ("GLFW Error %d: %s\n", code, msg);
}

static void LoadFonts (const std::vector<FontCfg> &cfgs, ImGuiIO &io)
{
    if (!cfgs.empty ())
    {
        for (auto &x: cfgs)
        {
            L_INFO ("Loading Font: %s", x.name.c_str ());
            bool first = true;
            for (auto &y: x.sub_fonts)
            {
                L_INFO ("  Add Font From %s", y.path.c_str ());
                ImFontConfig font_cfg;
                strncpy (font_cfg.Name, x.name.c_str (), std::size (font_cfg.Name) - 1);
                font_cfg.MergeMode        = !first;
                font_cfg.PixelSnapH       = true;
                font_cfg.FontBuilderFlags = (ImGuiFreeTypeBuilderFlags_Bitmap | ImGuiFreeTypeBuilderFlags_LoadColor);
                if (x.bold)
                    font_cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Bold;
                auto font = io.Fonts->AddFontFromFileTTF (y.path.c_str (), x.size, &font_cfg, y.ranges);
                LC_FATAL (!font, "Font Add Failed!");
                first = false;
            }
        }
    }
    else
    {
        L_INFO ("Load default fonts.");
        io.Fonts->AddFontDefault ();
    }
    bool ok = io.Fonts->Build ();
    LC_FATAL (!ok, "Font Build Failed!");
}

Application::Application (const char *title, s32 width, s32 height, u32 awf)
{
    LC_FATAL (instance != nullptr, "Application already instantiated!");

    glfwSetErrorCallback (&glfw_error_handler);
    LC_FATAL (glfwInit () != GLFW_TRUE, "glfw init error!");

    // GL 3.0 + GLSL 130
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWmonitor *monitor      = nullptr;
    auto const   window_state = (awf & AppWindowFlag_StateMask);
    if (window_state == AppWindowFlag_Fullscreen)
    {
        // fullscreen
        monitor = glfwGetPrimaryMonitor ();
        // update width and height
        glfwGetMonitorWorkarea (monitor, nullptr, nullptr, &width, &height);
    }
    else if (window_state == AppWindowFlag_Maximum)
    {
        glfwWindowHint (GLFW_MAXIMIZED, GLFW_TRUE);
    }
    else
    {
        // normal
        if (awf & AppWindowFlag_Frameless)
        {
            glfwWindowHint (GLFW_DECORATED, GLFW_FALSE);
        }
        if (awf & AppWindowFlag_Topmost)
        {
            glfwWindowHint (GLFW_FLOATING, GLFW_TRUE);
        }
        if (awf & AppWindowFlag_Opacity)
        {
            glfwWindowHint (GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        }
    }

    glfwWindowHint (GLFW_VISIBLE, GLFW_FALSE);

    auto wnd = glfwCreateWindow (width, height, title, monitor, nullptr);
    LC_FATAL (!wnd, "Window creation error!");
    window = wnd;

    glfwMakeContextCurrent (wnd);
    glfwSwapInterval (1);  // enable v-sync
}

void Application::ConfigImGui (ImGuiIO &io)
{
    io.IniFilename = nullptr;                              // disable state save
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
                                                           // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable
                                                           // Multi-Viewport / Platform Windows io.ConfigViewportsNoAutoMerge = true;
                                                           // io.ConfigViewportsNoTaskBarIcon = true;
}

void Application::ConfigStyle (ImGuiStyle &style)
{
    // Colors
    ImVec4 *colors                         = style.Colors;
    colors[ImGuiCol_Text]                  = ImVec4 (1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4 (0.63f, 0.63f, 0.63f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4 (0.15f, 0.22f, 0.30f, 1.00f);
    colors[ImGuiCol_ChildBg]               = ImVec4 (0.32f, 0.33f, 0.34f, 1.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4 (0.16f, 0.24f, 0.23f, 1.00f);
    colors[ImGuiCol_Border]                = ImVec4 (0.22f, 0.41f, 0.72f, 0.58f);
    colors[ImGuiCol_BorderShadow]          = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]               = ImVec4 (0.10f, 0.17f, 0.24f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4 (0.14f, 0.17f, 0.11f, 1.00f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4 (0.18f, 0.18f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBg]               = ImVec4 (0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4 (0.05f, 0.15f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4 (0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4 (0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4 (0.18f, 0.18f, 0.18f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4 (0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4 (0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4 (0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4 (0.93f, 0.93f, 0.93f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4 (0.93f, 0.93f, 0.93f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]      = ImVec4 (0.42f, 0.57f, 0.09f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4 (0.42f, 0.57f, 0.09f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4 (0.48f, 0.66f, 0.10f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4 (0.15f, 0.20f, 0.03f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4 (0.18f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4 (0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_HeaderActive]          = ImVec4 (0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4 (0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4 (0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4 (0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4 (0.66f, 0.81f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4 (0.76f, 0.87f, 1.00f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4 (0.83f, 0.91f, 1.00f, 0.95f);
    colors[ImGuiCol_Tab]                   = ImVec4 (0.37f, 0.42f, 0.42f, 0.86f);
    colors[ImGuiCol_TabHovered]            = ImVec4 (0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive]             = ImVec4 (0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused]          = ImVec4 (0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4 (0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_DockingPreview]        = ImVec4 (0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg]        = ImVec4 (0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]             = ImVec4 (0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4 (1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]         = ImVec4 (0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4 (1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]         = ImVec4 (0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]     = ImVec4 (0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight]      = ImVec4 (0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg]            = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4 (1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]        = ImVec4 (0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]        = ImVec4 (0.78f, 1.00f, 0.59f, 0.90f);
    colors[ImGuiCol_NavHighlight]          = ImVec4 (0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4 (1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4 (0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4 (0.80f, 0.80f, 0.80f, 0.35f);

    // Size
    style.FrameRounding     = 4.f;
    style.PopupRounding     = 4.f;
    style.ChildRounding     = 4.f;
    style.WindowRounding    = 4.f;
    style.GrabRounding      = 8.f;
    style.ScrollbarRounding = 8.f;
    style.WindowPadding     = {8.f, 8.f};
    style.FramePadding      = {6.f, 4.f};
    style.ItemSpacing       = {6.f, 4.f};
}

std::vector<FontCfg> Application::GetFontConfigs (ImGuiIO &io)
{
    return {};  // let system load default fonts
}

void Application::BeforeMainLoop ()
{
    // nothing
}

void Application::AfterMainLoop ()
{
    // nothing
}

void Application::SetClearColor(float r, float g, float b, float a)
{
    clear_color.x = r;
    clear_color.y = g;
    clear_color.z = b;
    clear_color.w = a;
}

void Application::Init ()
{
    IMGUI_CHECKVERSION ();

    ImGui::CreateContext ();
    ImPlot::CreateContext ();
    ImGuiIO &io = ImGui::GetIO ();

    // User: config imgui flags
    L_DEBUG ("Configuring ImGui");
    ConfigImGui (io);

    // User: Style
    L_DEBUG ("Configuring ImGui Styles");
    ConfigStyle (ImGui::GetStyle ());

    // backend inits
    ImGui_ImplGlfw_InitForOpenGL (window, true);
    ImGui_ImplOpenGL3_Init (glsl_version);

    L_INFO ("ImGui Init Done!");

    // User: Fonts
    L_INFO ("Preparing Load Fonts.");
    auto fonts = GetFontConfigs (io);
    LoadFonts (fonts, io);
}

int Application::Exec ()
{
    ImGuiIO     &io          = ImGui::GetIO ();
    double const started     = glfwGetTime ();
    double       last_update = started;

    // User: Post Loop
    L_INFO ("User Init Process...");
    BeforeMainLoop ();

    // show the window
    glfwShowWindow(window);

    L_INFO ("Enter MainLoop.");
    // loop
    while (!glfwWindowShouldClose (window))
    {
        double now        = glfwGetTime ();
        double delta_time = now - last_update;
        last_update       = now;
        double total_time = now - started;

        glfwPollEvents ();  // process glfw events

        // new frame
        ImGui_ImplGlfw_NewFrame ();
        ImGui_ImplOpenGL3_NewFrame ();
        ImGui::NewFrame ();

        // User Content
        Update (delta_time, total_time);

        // render
        ImGui::Render ();
        int display_w, display_h;
        glfwGetFramebufferSize (window, &display_w, &display_h);
        glViewport (0, 0, display_w, display_h);
        glClearColor (clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear (GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData ());

        // multi viewports
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext ();
            ImGui::UpdatePlatformWindows ();
            ImGui::RenderPlatformWindowsDefault ();
            glfwMakeContextCurrent (backup_current_context);
        }

        glfwSwapBuffers (window);
    }
    L_INFO ("Exiting MainLoop.");

    // User: End Loop
    L_INFO ("User Cleanup Process.");
    AfterMainLoop ();

    return exit_code;
}

Application::~Application ()
{
    // cleanup
    L_INFO ("Cleanup.");
    ImGui_ImplOpenGL3_Shutdown ();
    ImGui_ImplGlfw_Shutdown ();
    ImPlot::DestroyContext ();
    ImGui::DestroyContext ();
    glfwDestroyWindow (window);
    glfwTerminate ();

    instance = nullptr;
}

void Application::Exit (int code)
{
    glfwSetWindowShouldClose (window, GLFW_TRUE);
    exit_reqested = true;
    exit_code     = code;
}

Application *Application::Instance ()
{
    return instance;
}
