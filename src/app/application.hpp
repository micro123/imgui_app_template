#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "config.h"
#include "fw_export.h"
#include "imgui.h"
#include "font_config.hpp"
#include "core/log.hpp"

#include <memory>

enum AppWindowFlag: u32 {
    AppWindowFlag_Default = 0x0,

    AppWindowFlag_Normal = 0x10,
    AppWindowFlag_Maximum = 0x11,
    AppWindowFlag_Fullscreen = 0x12,
    AppWindowFlag_StateMask  = 0x13,

    AppWindowFlag_Frameless = 0x100,
    AppWindowFlag_Topmost = 0x200,
    AppWindowFlag_Opacity = 0x400,
    AppWindowFlag_NoResize = 0x800,
};

extern int main_proc();

class FW_API Application {
public:
    virtual ~Application();
    void Exit(int code);
    static Application* Instance();
protected:
    Application(const char *title, s32 width, s32 height, u32 awf = AppWindowFlag_Default);

    virtual void ConfigImGui(ImGuiIO &io);
    virtual void ConfigStyle(ImGuiStyle &style);
    virtual std::vector<FontCfg> GetFontConfigs(ImGuiIO &io);

    virtual void BeforeMainLoop();
    virtual void Update(double delta, double time) = 0;
    virtual void AfterMainLoop();

    void SetClearColor(float r, float g, float b, float a);
private:
    void Init();
    int Exec();

    friend int main_proc();
};

#define App (Application::Instance())

using PFN_CreateApplication = Application*(*)();

// Must Implement by user
#if defined(_WIN32)
extern  PFN_CreateApplication CreateApplication;
#define EntryPoint() \
extern "C" API_EXPORT Application* WIN_CreateApplication()
#define ENTRY_DECL() PFN_CreateApplication CreateApplication
#define ENTRY_INIT() do { *(void**)&CreateApplication = GetProcAddress(GetModuleHandleW(0), "WIN_CreateApplication"); LC_FATAL(CreateApplication == NULL, "unable to find entry symbol!"); } while(0)
#else
API_EXPORT
Application* CreateApplication();
#define EntryPoint() \
Application* CreateApplication()
#define ENTRY_INIT() do {} while(0)
#define ENTRY_DECL()
#endif

#endif /* APPLICATION_HPP */
