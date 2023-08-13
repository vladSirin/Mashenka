#include "Mashenka.h"

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
        MK_INFO("ExampleLayer::Update");
    }

    void OnEvent(Mashenka::Event& event) override
    {
        MK_TRACE("{0}", event);
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