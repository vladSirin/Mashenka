#include "Mashenka.h"
#include "imgui/imgui.h"

/*
 * 3. **Creating a Sandbox Class**:
 * A sandbox class is created as an instance of the Hazel application.
 * This class is defined in the client and returned by the `create application` function. 
 */
class ExampleLayer : public Mashenka::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
        
    }

    void OnUpdate() override
    {
        // MK_INFO("ExampleLayer::Update");
        if (Mashenka::Input::IsKeyPressed(MK_KEY_TAB))
        {
            MK_TRACE("Tab Key is Pressed (poll)!");
        }
    }

    virtual void OnImGuiRender() override
    {
        
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(Mashenka::Event& event) override
    {
        // MK_TRACE("{0}", event);
        if (event.GetEventType() == Mashenka::EventType::KeyPressed)
        {
            const auto& e = dynamic_cast<Mashenka::KeyPressedEvent&>(event);
            if (e.GetKeyCode() == MK_KEY_TAB)
            {
                MK_TRACE("Tab Key is Pressed (event)!");
            }
            MK_TRACE("{0}", static_cast<char>(e.GetKeyCode()));
        }
    }
};

class Sandbox : public Mashenka::Application
{
public:
     Sandbox()
     {
         PushLayer(new ExampleLayer);
     }

     ~Sandbox()
     {
          
     }
};

Mashenka::Application* Mashenka::CreateApplication()
{
    printf("Sandbox is now on!");
    return new Sandbox();
}