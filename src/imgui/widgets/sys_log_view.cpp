#include "sys_log_view.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <core/log_store.hpp>
#include <text/IconsFontAwesome6.h>

#include "imgui_ext.hpp"

using namespace ImGui;

SysLogView::SysLogView(const char *title): title_(title)
{
    
}

SysLogView::~SysLogView()
{
    
}

void SysLogView::Show()
{
    PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    bool open = Begin (title_);
    PopStyleVar ();
    if (open) {
        PushStyleVar (ImGuiStyleVar_CellPadding, ImVec2(0,8.f));
        PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2(0,2.f));

        static auto logger = log_store::instance();
        static bool auto_scroll = true;
        static const ImU32 level_colors[] {
                IM_COL32 (0xcd,0xcd,0xcd,0xff),
                IM_COL32 (0xff,0xff,0xff,0xff),
                IM_COL32 (0xcd,0xdc,0x39,0xff),
                IM_COL32 (0xff,0xea,0x00,0xff),
                IM_COL32 (0xff,0x17,0x44,0xff),
                IM_COL32 (0xff,0x00,0x00,0xff),
        };

        static u8 level;
        static char *payload_text = nullptr;
        static u32   payload_size = 0;
        ImGuiID detail_id = GetID("##PayloadDetails");

        PushStyleColor(ImGuiCol_ChildBg, GetStyleColorVec4 (ImGuiCol_Header));
        BeginChild("Filters", ImVec2(0, GetTextLineHeightWithSpacing()*1.5f));
        PopStyleColor();
        {
            static u32 filter_value = logger->filter_value();
            AlignTextToFramePadding();
            //FontManager::instance()->use_fonts(1); // bold
            PushStyleColor (ImGuiCol_Text, level_colors[0]);
            if (CheckboxFlags(ICON_FA_CODE, &filter_value, 1 << 0))
            {
                logger->set_filter(0, filter_value & 0x1);
            }
            SameLine(0,GetFontSize() * .5f);
            PushStyleColor (ImGuiCol_Text, level_colors[1]);
            if (CheckboxFlags(ICON_FA_BUG, &filter_value, 1 << 1))
            {
                logger->set_filter(1, filter_value & 0x2);
            }
            SameLine(0,GetFontSize() * .5f);
            PushStyleColor (ImGuiCol_Text, level_colors[2]);
            if (CheckboxFlags(ICON_FA_CIRCLE_INFO, &filter_value, 1 << 2))
            {
                logger->set_filter(2, filter_value & 0x4);
            }
            SameLine(0,GetFontSize() * .5f);
            PushStyleColor (ImGuiCol_Text, level_colors[3]);
            if (CheckboxFlags(ICON_FA_TRIANGLE_EXCLAMATION, &filter_value, 1 << 3))
            {
                logger->set_filter(3, filter_value & 0x8);
            }
            SameLine(0,GetFontSize() * .5f);
            PushStyleColor (ImGuiCol_Text, level_colors[4]);
            if (CheckboxFlags(ICON_FA_CIRCLE_XMARK, &filter_value, 1 << 4))
            {
                logger->set_filter(4, filter_value & 0x10);
            }
            SameLine(0,GetFontSize() * .5f);
            PushStyleColor (ImGuiCol_Text, level_colors[5]);
            if (CheckboxFlags(ICON_FA_GHOST, &filter_value, 1 << 5))
            {
                logger->set_filter(5, filter_value & 0x20);
            }
            PopStyleColor (6);

            // FontManager::instance()->use_fonts(0);

            auto const size1 = CalcTextSize ("\U0001f447");
            auto const size2 = CalcTextSize (ICON_FA_CLOCK_ROTATE_LEFT);
            auto const size3 = CalcTextSize (ICON_FA_TRASH);

            SameLine(GetWindowContentRegionMax().x - size1.x - size2.x - size3.x - GetFontSize() * 4.f);
            Checkbox ("\U0001f447", &auto_scroll);
            TextToolTipMaker("\U0001f447: 跟随最新一条日志显示");

            SameLine(GetWindowContentRegionMax().x - size2.x - size3.x - GetFontSize() * 2.f);
            if (Button(ICON_FA_CLOCK_ROTATE_LEFT)) {
                filter_value = 0xFF;
            }
            TextToolTipMaker(ICON_FA_CLOCK_ROTATE_LEFT ": 重置等级筛选器（显示所有日志）");

            SameLine(GetWindowContentRegionMax().x - size3.x - GetFontSize());
            PushStyleColor(ImGuiCol_Button, IM_COL32(210, 20, 20, 255));
            PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(225, 5, 5, 255));
            PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(250, 10, 10, 255));
            PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
            if (Button(ICON_FA_TRASH))
                logger->clear();
            PopStyleColor(4);
            TextToolTipMaker(ICON_FA_TRASH ": 删除所有日志");
        }
        EndChild();

        // log content
        PushStyleColor(ImGuiCol_ChildBg, 0);
        BeginChild("Content");
        PopStyleColor();

        if (BeginTable("Log Content", 5,
                        ImGuiTableFlags_ContextMenuInBody |
                                ImGuiTableFlags_SizingStretchSame |
                                ImGuiTableFlags_ScrollY |
                                ImGuiTableFlags_RowBg
                        ))
        {
            // FontManager::instance()->use_fonts(1);
            TableSetupColumn(" 日期", ImGuiTableColumnFlags_WidthFixed, GetFontSize()*12.f);
            TableSetupColumn(" 线程", ImGuiTableColumnFlags_WidthFixed, GetFontSize()*4.f);
            TableSetupColumn(" 等级", ImGuiTableColumnFlags_WidthFixed, GetFontSize()*3.f);
            TableSetupColumn(" 消息", ImGuiTableColumnFlags_WidthStretch);
            TableSetupColumn(" 操作", ImGuiTableColumnFlags_WidthFixed, GetFontSize()*6.f);

            TableSetupScrollFreeze(0, 1); // pin header row
            TableHeadersRow();
            // FontManager::instance()->use_fonts(0);

            // log content
            auto iter = logger->logs();
            ImGuiListClipper clipper;
            clipper.Begin(iter.count());

            static const char *level_text[] {
                    "  " ICON_FA_CODE, "追踪",
                    "  " ICON_FA_BUG, "调试",
                    "  " ICON_FA_CIRCLE_INFO, "消息",
                    "  " ICON_FA_TRIANGLE_EXCLAMATION, "警告",
                    "  " ICON_FA_CIRCLE_XMARK, "错误",
                    "  " ICON_FA_GHOST, "致命",
            };

            while(clipper.Step()) {
                for (int i=clipper.DisplayStart; i<clipper.DisplayEnd; ++i) {

                    auto const log = iter[i];
                    if (!log) {
                        clipper.End();
                        break;
                    }

                    PushID(log);
                    PushStyleColor (ImGuiCol_Text, level_colors[log->level]);

                    TableNextColumn();
                    Dummy(ImVec2{GetFontSize(), 0});
                    SameLine();
                    TextUnformatted(log->datetime);
                    TableNextColumn();
                    Dummy(ImVec2{GetFontSize(), 0});
                    SameLine();
                    Text("%lu", log->thread_id);
                    TableNextColumn();
                    Text("%s", level_text[log->level * 2]);
                    if (IsItemHovered ())
                        SetTooltip ("%s", level_text[log->level*2 + 1]);
                    TableNextColumn();
                    TextUnformatted (log->payload, strchr (log->payload, '\n'));

                    PopStyleColor ();
                    
                    TableNextColumn();
                    Dummy(ImVec2(GetFontSize(), GetFontSize()));
                    SameLine();
                    if (Button(ICON_FA_CLIPBOARD)) {
                        SetClipboardText(log->payload);
                    }
                    TextToolTipMaker("复制消息");
                    SameLine(0, GetFontSize());
                    if (Button(ICON_FA_UP_RIGHT_FROM_SQUARE))
                    {
                        level = log->level;
                        payload_text = (char*)log->payload;
                        payload_size = log->payload_size;
                        OpenPopup(detail_id);
                    }
                    TextToolTipMaker("在独立窗口显示");
                    PopID();
                }
            }
            if (auto_scroll) {
                SetScrollY (GetScrollMaxY());
            }
            EndTable();
        }
        EndChild();
        PopStyleVar (2);

        CenterWindowPercent(.6f);
        if (BeginPopup("##PayloadDetails")) {
            PushStyleColor(ImGuiCol_Text, level_colors[level]);
            InputTextMultiline("##Details", payload_text, payload_size, ImVec2(-1,-1), ImGuiInputTextFlags_ReadOnly);
            PopStyleColor();
            EndPopup();
        }
    }
    End ();
}
