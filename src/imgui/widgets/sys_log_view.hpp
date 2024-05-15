#ifndef SYS_LOG_VIEW_HPP
#define SYS_LOG_VIEW_HPP

#include <imgui.h>

class IMGUI_API SysLogView {
public:
    SysLogView(const char *title);
    ~SysLogView();

    void Show();
private:
    const char *title_;
};

#endif /* SYS_LOG_VIEW_HPP */
