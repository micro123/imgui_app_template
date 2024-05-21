#include <text/IconsFontAwesome6.h>
#include <app/application.hpp>
#include <memory>
#include <imgui.h>
#include <implot.h>
#include <utils/path_tools.hpp>
#include "core/log.hpp"
#include "imgui/widgets/ImFileDialog.h"
#include "imgui/widgets/calendar.hpp"
#include "imgui/widgets/sys_log_view.hpp"
#include "imgui/widgets/texture.h"
#include "imgui/widgets/imgui_notify.h"
#include "imgui/utils/dock_helpers.hpp"
#include "job/job_system.hpp"

extern void SomeCrashFunction();
extern void TestHttpGet();

class MyApp final : public Application
{
public:
    MyApp () : Application ("Hello World", 1440, 900, AppWindowFlag_Topmost) {}

protected:
    void BeforeMainLoop() override
    {
        some_pic.LoadFromAsset("images/smile.png");
    }
    
    void Update (double delta, double total) override
    {
        using namespace ImGui;

        static bool demo_imgui  = false;
        static bool demo_implot = false;
        static float rgba[4] {0,0,0,1};

        CreateRootDockNode();

        if (Begin ("First Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Text("Delta: %.3f", delta);
            Text("Total: %.3f", total);

            SeparatorText("Simple Widgets");

            TextUnformatted ("Hello World!");
            TextUnformatted ("我是简体中文！");
            if (ColorEdit4("Clear Color", rgba)) {
                SetClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
            }

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

            Separator();
            if (Button("TestHttpGet")) {
                submit_job(create_job(TestHttpGet));
            }

            Separator();
            static char some_str[128] {};
            InputText("请输入一些东西", some_str, sizeof(some_str));
        }
        End ();

        if(Begin("My Widgets", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static bool editor_modal = true;
            static std::string date_text = Date::Current();
            static DateEditor  editor_popup("SomeDay");

            if (editor_popup.RenderAsPopup(editor_modal))
                date_text = editor_popup.GetSelectedDate();
            Checkbox("使用模态对话框", &editor_modal);

            AlignTextToFramePadding();
            Text("日期编辑：%s", date_text.c_str());
            SameLine();
            if (Button ("选择"))
            {
                editor_popup.Show();
            }

            Separator();
            static std::string date_text2 = Date::Current();
            static DateEditor  editor_child("AsChild");

            Text("直接编辑：%s", date_text2.c_str());
            if (editor_child.RenderAsChild())
                date_text2 = editor_child.GetSelectedDate();

            Separator();
            Text("Image %dx%d", some_pic.Width(), some_pic.Height());
            SameLine();
            if (Button("选择文件")) {
                FDLG.Open("SelectImage", "选择图片", "Image Files {.jpg,.png}");
            }
            Image(some_pic, ImVec2(.25f,.25f));

            if (FDLG.IsDone("SelectImage")) {
                if (FDLG.HasResult()) {
                    auto path = FDLG.GetResult().string();
                    some_pic.LoadFromFile(path.c_str());
                    InsertNotification(ImGuiToast(ImGuiToastType_Info, "load image %s", path.c_str()));
                }
                FDLG.Close();
            }
        }
        End();

        if (demo_imgui)
            ImGui::ShowDemoWindow (&demo_imgui);
        if (demo_implot)
            ImPlot::ShowDemoWindow (&demo_implot);

        static SysLogView view("System Log");
        
        SetNextWindowSize(ImVec2(600, 250));
        view.Show();
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

private:
    Texture some_pic;
};

EntryPoint()
{
    L_INFO("Application Creation!");
    return new MyApp;
}
