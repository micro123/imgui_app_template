#include <imgui_node_editor.h>
#include "core/log.hpp"
#include "page_base.hpp"

namespace ed = ax::NodeEditor;

class NodeEditorPage final : public Page
{
public:
    void OnStart () override
    {
        context_ = ed::CreateEditor ();
    }

    void OnStop () override
    {
        ed::DestroyEditor (context_);
    }

    void OnFrame (double delta, double total) override
    {
        ed::SetCurrentEditor (context_);
        ed::Begin ("MyNodes");
        int uniqueId = 1;
        ed::BeginNode (uniqueId++);
        ImGui::Text ("Node A");
        ed::BeginPin (uniqueId++, ed::PinKind::Input);
        ImGui::Text ("-> In");
        ed::EndPin ();
        ImGui::SameLine ();
        ed::BeginPin (uniqueId++, ed::PinKind::Output);
        ImGui::Text ("Out ->");
        ed::EndPin ();
        ed::EndNode();

        // handle creation
        if (ed::BeginCreate())
        {
            ed::PinId in,out;
            if(ed::QueryNewLink(&in, &out) && in && out) {
                if (ed::AcceptNewItem())
                    L_INFO("%ld -> %ld", in.Get(), out.Get());
            }
        }
        ed::EndCreate();

        // handle deletion
        if (ed::BeginDelete())
        {

        }
        ed::EndDelete();

        ed::End ();
        ed::SetCurrentEditor (nullptr);
    }

    const char *Name () const override
    {
        return "Node Editor";
    }

private:
    ed::EditorContext *context_;
};

REGISTER_PAGE (NodeEditorPage);
