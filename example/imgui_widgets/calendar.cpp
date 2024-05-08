#include "calendar.hpp"
#include <fmt/format.h>

#include <chrono>
#include <ctime>

#include <imgui_internal.h>

using namespace ImGui;

Date::operator std::string () const
{
    return fmt::format("{}-{:02d}-{:02d}", year, month, day);
}

Date Date::Current ()
{
    auto const now = std::chrono::system_clock::now();
    auto const t = std::chrono::system_clock::to_time_t(now);

#if defined(_MSC_VER)
    std::tm tm;
    (void)localtime_s(&tm, &t);
#else
    std::tm tm = *std::localtime(&t);
#endif

    return { tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday };
}

enum GridFlag : int
{
    GridFlag_Selected = 1,  // 选中
    GridFlag_Today    = 2,  // 今天
    GridFlag_GrayOut  = 4,  // 暗色显示
};

void DateEditor::DateGrid::UpdateFlags (const Date &disp, const Date &sel, const Date &today)
{
    flags = 0;
    auto const date = *this;
    if (sel == date)
    {
        flags |= GridFlag_Selected;
    }
    if (month != disp.month)
    {
        flags |= GridFlag_GrayOut;
    }
    else if (today == date)
    {
        flags |= GridFlag_Today;
    }
}

DateEditor::DateEditor (const char *title_, Date initial_value): title_ (title_), display_date_ (initial_value), selected_ (initial_value)
{
    popup_id_ = GetID(title_);
    memset(buffer_, 0, sizeof(buffer_));
    RefreshBuffers();
}

Date DateEditor::GetSelectedDate () const
{
    return selected_;
}

void DateEditor::Show () const
{
    OpenPopup(popup_id_);
}

static bool CircularInt(const char *label, int *v, int min, int max, int step, int step_fast)
{
    int        v_old   = *v;
    const bool changed = InputInt(label, &v_old, step, step_fast);
    if (changed)
    {
        if(v_old < min)
        {
            *v = max;
        }
        else if (v_old > max)
        {
            *v = min;
        }
        else
        {
            *v = v_old;
        }
    }
    return changed;
}

bool DateEditor::Render ()
{
    accept_ = false;
    if (BeginPopup(title_, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration))
    {
        AlignTextToFramePadding();

        auto const font_size = GetFontSize();

        TextUnformatted("年："); SameLine();
        SetNextItemWidth(font_size * 8);
        if (InputInt ("##Year", &display_date_.year, 1, 1))
        {
            RefreshBuffers();
        }
        SameLine();
        TextUnformatted("月："); SameLine();
        SetNextItemWidth(font_size * 8);
        if (CircularInt ("##Month", &display_date_.month, 1, 12, 1, 1))
        {
            RefreshBuffers();
        }

        SameLine(0, font_size);
        if (Button ("今天"))
        {
            SelectToday();
        }
        SameLine(0, font_size);
        if (Button ("确定"))
        {
            accept_ = true;
            CloseCurrentPopup();
        }

        if (BeginTable ("##DisplayDate", 7, ImGuiTableFlags_Borders))
        {
            auto const item_width = font_size * 4;
            TableSetupColumn("星期日", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableSetupColumn("星期一", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableSetupColumn("星期二", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableSetupColumn("星期三", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableSetupColumn("星期四", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableSetupColumn("星期五", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableSetupColumn("星期六", ImGuiTableColumnFlags_WidthFixed, item_width);
            TableHeadersRow();

            bool changed = false;
            for (size_t i=0; i<std::size(buffer_); ++i)
            {
                TableNextColumn();
                auto const& g = buffer_[i];
                PushID(&g);
                if (RenderDateGrid(g))
                {
                    selected_ = g;
                    changed = true;
                }
                PopID();
            }
            EndTable();

            if (changed)
            {
                // check if display month not equals selected month
                if (display_date_.month != selected_.month)
                {
                    display_date_ = selected_;
                }
                RefreshBuffers();
            }
        }

        EndPopup();
    }
    return accept_;
}

void DateEditor::SelectToday ()
{
    display_date_ = selected_ = Date::Current();
    RefreshBuffers();
}

static std::tm from_ymd(int y, int m, int d)
{
    std::tm tm{};
    tm.tm_year = y - 1900;
    tm.tm_mon = m - 1;
    tm.tm_mday = d;
    std::time_t t = std::mktime(&tm);

#if defined(_MSC_VER)
    (void)localtime_s(&tm, &t);
#else
    tm = *std::localtime(&t);
#endif

    tm.tm_year += 1900;
    tm.tm_mon += 1;

    return tm;
}

static int CircularInc(int v, int min, int max)
{
    if (v == max)
        v = min;
    else
        ++v;
    return v;
}
static int CircularDec(int v, int min, int max)
{
    if (v == min)
        v = max;
    else
        --v;
    return v;
}

constexpr int month_days[]{
    0,0,   // padding
    31,31, // 1
    28,29, // 2
    31,31, // 3
    30,30, // 4
    31,31, // 5
    30,30, // 6
    31,31, // 7
    31,31, // 8
    30,30, // 9
    31,31, // 10
    30,30, // 11
    31,31, // 12
};

static int GetMonthDays(int y, int m)
{
    int const is_leap = y % 4 == 0 && y % 100 != 0 || y % 400 == 0;
    return month_days[m * 2 + is_leap];
}

void DateEditor::RefreshBuffers ()
{
    // fill buffers by display_date_.{year,month}
    auto first = from_ymd(display_date_.year, display_date_.month, 1);
    int fill_y, fill_m, fill_d;
    auto pb = buffer_;
    auto const pe = pb + std::size(buffer_);
    auto const today = Date::Current();

    // 上个月
    fill_m = CircularDec(display_date_.month, 1, 12);
    fill_y = display_date_.year - static_cast<int> (fill_m == 12);
    fill_d = GetMonthDays(fill_y, fill_m) - first.tm_wday + 1;
    for (int i=0; i<first.tm_wday; ++i)
    {
        pb->year = fill_y;
        pb->month = fill_m;
        pb->day = fill_d++;
        pb->UpdateFlags(display_date_, selected_, today);
        ++pb;
    }
    
    // 当前月
    fill_y = display_date_.year;
    fill_m = display_date_.month;
    fill_d = 1;
    for (int i=0, max = GetMonthDays (fill_y, fill_m); i<max; ++i)
    {
        pb->year = fill_y;
        pb->month = fill_m;
        pb->day = fill_d++;
        pb->UpdateFlags(display_date_, selected_, today);
        ++pb;
    }

    // 下个月
    fill_m = CircularInc (display_date_.month, 1, 12);
    fill_y = display_date_.year + static_cast<int>(fill_m == 1);
    fill_d = 1;
    while(pb < pe)
    {
        pb->year = fill_y;
        pb->month = fill_m;
        pb->day = fill_d++;
        pb->UpdateFlags(display_date_, selected_, today);
        ++pb;
    }
}

bool DateEditor::RenderDateGrid (const DateGrid &dg)
{
    static char buffer[16];

    // styles
    int colors = 2;
    PushStyleColor(ImGuiCol_ButtonActive, GetStyleColorVec4(ImGuiCol_Button));
    PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32 (250, 24, 16, 255));
    if (dg.flags & GridFlag_GrayOut)
    {
        PushStyleColor(ImGuiCol_Button, IM_COL32(104, 104, 104, 255));
        ++colors;
    }
    if (dg.flags & GridFlag_Selected)
    {
        PushStyleColor(ImGuiCol_Button, IM_COL32(250, 12, 8, 255));
        ++colors;
    }
    else if (dg.flags & GridFlag_Today)
    {
        PushStyleColor(ImGuiCol_Button, IM_COL32(255, 147, 0, 255));
        ++colors;
    }

    if (dg.day == 1)
        (void)snprintf(buffer, sizeof(buffer), "%02d月", dg.month);
    else
        (void)snprintf(buffer, sizeof(buffer), "%02d日", dg.day);
    const bool clicked = Button(buffer);
    PopStyleColor(colors);
    return clicked;
}
