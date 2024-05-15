//
// Created by tang on 22-6-8.
//

#include "imgui_notify.h"

#include <cstdio>
#include <vector>
#include "text/IconsFontAwesome6.h"

#define NOTIFY_PADDING_X                20.f        // Bottom-left X padding
#define NOTIFY_PADDING_Y                20.f        // Bottom-left Y padding
#define NOTIFY_PADDING_MESSAGE_Y        10.f        // Padding Y between each message
#define NOTIFY_FADE_IN_OUT_TIME            150            // Fade in and out duration

#define NOTIFY_OPACITY                    1.0f        // 0-1 Toast opacity
#define NOTIFY_TOAST_FLAGS                (ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_Tooltip)
// Comment out if you don't want any separator between title and content
#define NOTIFY_USE_SEPARATOR

#define NOTIFY_NULL_OR_EMPTY(str)        (!str ||! strlen(str))
#define NOTIFY_FORMAT(fn, format, ...)    if (format) { va_list args; va_start(args, format); fn(format, args, ## __VA_ARGS__); va_end(args); }


void ImGuiToast::set_title(const char *format, va_list args) {
    vsnprintf(this->title, sizeof(this->title), format, args);
}

void ImGuiToast::set_content(const char *format, va_list args) {
    vsnprintf(this->content, sizeof(this->content), format, args);
}

int64_t ImGuiToast::GetTickCount() { return int64_t(ImGui::GetTime() * 1000); }

auto ImGuiToast::set_title(const char *format, ...) -> void { NOTIFY_FORMAT(this->set_title, format) }

auto ImGuiToast::set_content(const char *format, ...) -> void { NOTIFY_FORMAT(this->set_content, format) }

auto ImGuiToast::set_type(const ImGuiToastType &type) -> void {
    IM_ASSERT(type < ImGuiToastType_COUNT);
    this->type = type;
}

auto ImGuiToast::get_title() -> char * { return this->title; }

const char* ImGuiToast::get_default_title() {
    if (!strlen(this->title)) {
        switch (this->type) {
            case ImGuiToastType_None:
                return nullptr;
            case ImGuiToastType_Success:
                return "Success";
            case ImGuiToastType_Warning:
                return "Warning";
            case ImGuiToastType_Error:
                return "Error";
            case ImGuiToastType_Info:
                return "Info";
        }
    }

    return this->title;
}

auto ImGuiToast::get_type() -> ImGuiToastType { return this->type; }

auto ImGuiToast::get_color() -> ImVec4 {
    switch (this->type) {
        case ImGuiToastType_None:
            return {255, 255, 255, 255}; // White
        case ImGuiToastType_Success:
            return {0, 255, 0, 255}; // Green
        case ImGuiToastType_Warning:
            return {255, 255, 0, 255}; // Yellow
        case ImGuiToastType_Error:
            return {255, 0, 0, 255}; // Error
        case ImGuiToastType_Info:
            return {0, 157, 255, 255}; // Blue
    }
    return {255, 255, 255, 255}; // White
}

auto ImGuiToast::get_icon() -> const char * {
    switch (this->type) {
        case ImGuiToastType_None:
            return nullptr;
        case ImGuiToastType_Success:
            return ICON_FA_CIRCLE_CHECK;
        case ImGuiToastType_Warning:
            return ICON_FA_TRIANGLE_EXCLAMATION;
        case ImGuiToastType_Error:
            return ICON_FA_CIRCLE_XMARK;
        case ImGuiToastType_Info:
            return ICON_FA_CIRCLE_INFO;
    }
    return nullptr;
}

auto ImGuiToast::get_phase() -> ImGuiToastPhase {
    const auto elapsed = get_elapsed_time();

    if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time + NOTIFY_FADE_IN_OUT_TIME) {
        return ImGuiToastPhase_Expired;
    } else if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time) {
        return ImGuiToastPhase_FadeOut;
    } else if (elapsed > NOTIFY_FADE_IN_OUT_TIME) {
        return ImGuiToastPhase_Wait;
    } else {
        return ImGuiToastPhase_FadeIn;
    }
}

auto ImGuiToast::get_fade_percent() -> float {
    const auto phase = get_phase();
    const auto elapsed = get_elapsed_time();

    if (phase == ImGuiToastPhase_FadeIn) {
        return ((float) elapsed / (float) NOTIFY_FADE_IN_OUT_TIME) * NOTIFY_OPACITY;
    } else if (phase == ImGuiToastPhase_FadeOut) {
        return (1.f - (((float) elapsed - (float) NOTIFY_FADE_IN_OUT_TIME - (float) this->dismiss_time) /
                       (float) NOTIFY_FADE_IN_OUT_TIME)) * NOTIFY_OPACITY;
    }

    return 1.f * NOTIFY_OPACITY;
}

ImGuiToast::ImGuiToast(ImGuiToastType type, int dismiss_time) {
    IM_ASSERT(type < ImGuiToastType_COUNT);

    this->type = type;
    this->dismiss_time = dismiss_time;
    this->creation_time = GetTickCount();

    memset(this->title, 0, sizeof(this->title));
    memset(this->content, 0, sizeof(this->content));
}

ImGuiToast::ImGuiToast(ImGuiToastType type, const char *format, ...) : ImGuiToast(type) {
    NOTIFY_FORMAT(this->set_content, format)
}

ImGuiToast::ImGuiToast(ImGuiToastType type, int dismiss_time, const char *format, ...) : ImGuiToast(type, dismiss_time) {
    NOTIFY_FORMAT(this->set_content, format)
}

namespace ImGui {
    std::vector<ImGuiToast> notifications;

    void InsertNotification(const ImGuiToast &toast) {
        notifications.push_back(toast);
    }

    void RemoveNotification(int index) {
        notifications.erase(notifications.begin() + index);
    }

    void RenderNotifications() {
        const auto vp_size = GetMainViewport()->Size;

        float height = 0.f;

        for (auto i = 0u; i < notifications.size(); i++) {
            auto *current_toast = &notifications[i];

            // Remove toast if expired
            if (current_toast->get_phase() == ImGuiToastPhase_Expired) {
                RemoveNotification((int)i);
                continue;
            }

            // Get icon, title and other data
            const auto icon = current_toast->get_icon();
            const auto title = current_toast->get_title();
            const auto content = current_toast->get_content();
            const auto default_title = current_toast->get_default_title();
            const auto opacity = current_toast->get_fade_percent(); // Get opacity based of the current phase

            // Window rendering
            auto text_color = current_toast->get_color();
            text_color.w = opacity;

            // Generate new unique name for this toast
            char window_name[50];
            sprintf(window_name, "##TOAST%d", i);

            //PushStyleColor(ImGuiCol_Text, text_color);
            SetNextWindowBgAlpha(opacity);
            SetNextWindowPos(ImVec2(vp_size.x - NOTIFY_PADDING_X, vp_size.y - NOTIFY_PADDING_Y - height),
                             ImGuiCond_Always, ImVec2(1.0f, 1.0f));
            Begin(window_name, nullptr, NOTIFY_TOAST_FLAGS);

            // Here we render the toast content
            {
                PushTextWrapPos(vp_size.x /
                                3.f); // We want to support multi-line text, this will wrap the text after 1/3 of the screen width

                bool was_title_rendered = false;

                // If an icon is set
                if (!NOTIFY_NULL_OR_EMPTY(icon)) {
                    //Text(icon); // Render icon text
                    TextColored(text_color, "%s", icon);
                    was_title_rendered = true;
                }

                // If a title is set
                if (!NOTIFY_NULL_OR_EMPTY(title)) {
                    // If a title and an icon is set, we want to render on same line
                    if (!NOTIFY_NULL_OR_EMPTY(icon))
                        SameLine();

                    TextUnformatted(title); // Render title text
                    was_title_rendered = true;
                } else if (!NOTIFY_NULL_OR_EMPTY(default_title)) {
                    if (!NOTIFY_NULL_OR_EMPTY(icon))
                        SameLine();

                    TextUnformatted(default_title); // Render default title text (ImGuiToastType_Success -> "Success", etc...)
                    was_title_rendered = true;
                }

                // In case ANYTHING was rendered in the top, we want to add a small padding so the text (or icon) looks centered vertically
                if (was_title_rendered && !NOTIFY_NULL_OR_EMPTY(content)) {
                    SetCursorPosY(GetCursorPosY() + 5.f); // Must be a better way to do this!!!!
                }

                // If a content is set
                if (!NOTIFY_NULL_OR_EMPTY(content)) {
                    if (was_title_rendered) {
#ifdef NOTIFY_USE_SEPARATOR
                        Separator();
#endif
                    }

                    TextUnformatted(content); // Render content text
                }

                PopTextWrapPos();
            }

            // Save height for next toasts
            height += GetWindowHeight() + NOTIFY_PADDING_MESSAGE_Y;

            // End
            End();
        }
    }
}

int64_t ImGuiToast::get_elapsed_time() { return GetTickCount() - this->creation_time; }

auto ImGuiToast::get_content() -> char * { return this->content; }
