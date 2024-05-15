// imgui-notify by patrickcjk
// https://github.com/patrickcjk/imgui-notify

#ifndef IMGUI_NOTIFY
#define IMGUI_NOTIFY

#include <cstdint>
#include <cstdarg>

#include "imgui.h"

#define NOTIFY_MAX_MSG_LENGTH            4096        // Max message content length
#define NOTIFY_DEFAULT_DISMISS           3000        // Auto dismiss after X ms (default, applied only of no data provided in constructors)


typedef int ImGuiToastType;
typedef int ImGuiToastPhase;
typedef int ImGuiToastPos;

enum ImGuiToastType_ {
    ImGuiToastType_None,
    ImGuiToastType_Success,
    ImGuiToastType_Warning,
    ImGuiToastType_Error,
    ImGuiToastType_Info,
    ImGuiToastType_COUNT
};

enum ImGuiToastPhase_ {
    ImGuiToastPhase_FadeIn,
    ImGuiToastPhase_Wait,
    ImGuiToastPhase_FadeOut,
    ImGuiToastPhase_Expired,
    ImGuiToastPhase_COUNT
};

enum ImGuiToastPos_ {
    ImGuiToastPos_TopLeft,
    ImGuiToastPos_TopCenter,
    ImGuiToastPos_TopRight,
    ImGuiToastPos_BottomLeft,
    ImGuiToastPos_BottomCenter,
    ImGuiToastPos_BottomRight,
    ImGuiToastPos_Center,
    ImGuiToastPos_COUNT
};

class IMGUI_API ImGuiToast {
private:
    ImGuiToastType type = ImGuiToastType_None;
    char title[NOTIFY_MAX_MSG_LENGTH]{};
    char content[NOTIFY_MAX_MSG_LENGTH]{};
    int dismiss_time = NOTIFY_DEFAULT_DISMISS;
    int64_t creation_time = 0;

private:
    // Setters

    void set_title(const char *format, va_list args);

    void set_content(const char *format, va_list args);

public:
    static int64_t GetTickCount();

    auto set_title(const char *format, ...) -> void;

    auto set_content(const char *format, ...) -> void;

    auto set_type(const ImGuiToastType &type) -> void;;

public:
    // Getters

    auto get_title() -> char *;;

    auto get_default_title() -> const char *;;

    auto get_type() -> ImGuiToastType;;

    auto get_color() -> ImVec4;

    auto get_icon() -> const char *;

    auto get_content() -> char *;;

    int64_t get_elapsed_time();

    auto get_phase() -> ImGuiToastPhase;

    auto get_fade_percent() -> float;

public:
    // Constructors

    explicit ImGuiToast(ImGuiToastType type, int dismiss_time = NOTIFY_DEFAULT_DISMISS);

    ImGuiToast(ImGuiToastType type, const char *format, ...);

    ImGuiToast(ImGuiToastType type, int dismiss_time, const char *format, ...);
};

namespace ImGui {
    /// <summary>
    /// Insert a new toast in the list
    /// </summary>
    void IMGUI_API InsertNotification(const ImGuiToast &toast);

    /// <summary>
    /// Remove a toast from the list by its index
    /// </summary>
    /// <param name="index">index of the toast to remove</param>
    void RemoveNotification(int index);

    /// <summary>
    /// Render toasts, call at the end of your rendering!
    /// </summary>
    void RenderNotifications();
}

#endif
