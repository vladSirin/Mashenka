#include "Mashenka.h"

/*
 * 3. **Creating a Sandbox Class**:
 * A sandbox class is created as an instance of the Hazel application.
 * This class is defined in the client and returned by the `create application` function. 
 */

class Sandbox : public Mashenka::Application
{
public:
     Sandbox()
     {
          
     }

     ~Sandbox()
     {
          
     }
};

Mashenka::Application* Mashenka::CreateApplication()
{
     return new Sandbox();
}