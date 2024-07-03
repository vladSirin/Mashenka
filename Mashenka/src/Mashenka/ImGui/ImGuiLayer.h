#pragma once
#include "Mashenka/Core/Layer.h"

namespace Mashenka
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& e) override;
        void OnImGuiRender() override; // for render specific things and debugging UI

        // Begin and End for the rendering prepare and execution.
        void Begin();
        void End();

        // setting if ImGui will block events
        void BlockEvents(bool block) { m_BlockEvents = block; }

    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };
}
