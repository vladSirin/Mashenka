// Engine: Mashenka Game Engine
#include "Mashenka.h"
#include "Mashenka/Core/EntryPoint.h"

// sandbox2D
#include "ExampleLayer.h"
#include "Sandbox2D.h"


// Should not include anything else than the engine to make it work, below is TEMP


/*
 * 3. **Creating a Sandbox Class**:
 * A sandbox class is created as an instance of the Mashenka application.
 * This class is defined in the client and returned by the `create application` function. 
 */

/* ==
 * == SandBox App for the Mashenka Game Engine ====================
 */

// The Sandbox class is the client application that using the engine
class Sandbox : public Mashenka::Application
{
public:
    Sandbox()
    {
        //PushLayer(new ExampleLayer);
        PushLayer(new Sandbox2D());
    }

    // destructor
    ~Sandbox() = default;
};

Mashenka::Application* Mashenka::CreateApplication()
{
    printf("Sandbox is now on!");
    return new Sandbox();
}
