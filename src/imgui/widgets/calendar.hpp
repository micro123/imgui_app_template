#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <imgui.h>
#include <string>

struct IMGUI_API Date {
    int year;
    int month;
    int day;

    operator std::string() const;
    bool operator== (const Date&) const = default;
    static Date Current();
};

class IMGUI_API DateEditor {
    struct DateGrid
    {
        int year, month, day, flags;

        operator Date() const { return { year, month, day }; }

        void UpdateFlags(const Date& disp, const Date& sel, const Date& today);
    };
public:
    DateEditor(const char *title_, Date initial_value = Date::Current());
    Date GetSelectedDate() const;
    void Show() const;
    bool RenderAsPopup(bool modal = false);
    bool RenderAsChild();
private:
    void SelectToday();
    void RefreshBuffers();
    void RenderContent(bool popup);
    static bool RenderDateGrid(const DateGrid& dg);

    const char* title_;         // 窗口标题
    Date        display_date_;  // 展示的月份
    Date        selected_;      // 选中的日期
    ImGuiID     popup_id_;      // 内部用
    DateGrid    buffer_[7*6];   // 日期信息
    int         display_cnt_;   // 需要显示多少个格子
    bool        accept_;        // 点击了确定
};

#endif /* CALENDAR_HPP */
