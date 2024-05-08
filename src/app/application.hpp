#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "config.h"
#include "imgui.h"
#include "font_config.hpp"

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
};

extern int main_proc();

class Application {
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

using UniqueApp = std::unique_ptr<Application>;

#define App (Application::Instance())

// Must Implement by user
UniqueApp CreateApplication();

#endif /* APPLICATION_HPP */
