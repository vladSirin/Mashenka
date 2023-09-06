#pragma once
#include "Mashenka/Layer.h"
#include "Mashenka/Events/KeyEvent.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "Mashenka/Events/MouseEvent.h"

namespace Mashenka
{
    class MASHENKA_API ImGuiLayer: public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override; // for render specific things and debugging UI

        // Begin and End for the rendering prepare and execution.
        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
        
    };
}

