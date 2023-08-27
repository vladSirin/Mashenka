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
        void OnUpdate() override;
        void OnEvent(Event& event) override;

    private:
        float m_Time = 0.0f;
    private:
        // Those events will be called layer by layer in the application class
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnMouseButtonRleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnWindowResizedEvent(WindowResizeEvent& e);
        
    };
}

