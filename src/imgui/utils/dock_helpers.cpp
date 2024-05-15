#include "dock_helpers.hpp"

#include <imgui_internal.h>

namespace ImGui
{

    void CreateRootDockNode ()
    {
        ImGuiViewport *viewport = GetMainViewport ();

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags   window_flags    = ImGuiWindowFlags_NoDocking;

        // fullscreen dockspace
        SetNextWindowPos (viewport->WorkPos);
        SetNextWindowSize (viewport->WorkSize);
        SetNextWindowViewport (viewport->ID);
        PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
        PushStyleVar (ImGuiStyleVar_WindowRounding, 0.0f);
        PushStyleVar (ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        window_flags |= ImGuiWindowFlags_NoBackground;
        window_flags |= ImGuiWindowFlags_NoNav;
        window_flags |= ImGuiWindowFlags_NoDecoration;

        Begin ("##DockspaceWin", nullptr, window_flags);

        PopStyleVar (3);

        ImGuiID dockSpaceId = ImGui::GetID ("MyDockSpace");
        DockSpace (dockSpaceId, ImVec2 (0.0f, 0.0f), dockspace_flags);

        End();
    }

}  // namespace ImGui
