#include <cstring>
#include "page_base.hpp"
#include <imgui.h>

class SimplePage final: public Page
{
public:
    SimplePage(const char *text, int i): x(i) {
        strncpy(text_buffer, text, sizeof(text_buffer));
    }

    void OnFrame(double delta, double total) override {
        ImGui::InputText("文本", text_buffer, sizeof(text_buffer));
        ImGui::InputInt("数字", &x);
    }
    const char *Name() const override { return "Simple"; }

private:
    char text_buffer[512];
    int  x;
};

REGISTER_PAGE(SimplePage, "Hello World!", 42);
