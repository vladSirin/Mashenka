// Engine: Mashenka Game Engine
#include "Mashenka.h"
#include "Mashenka/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Mashenka
{
    class Tamara : public Application
    {
    public:
        Tamara() : Application("Tamara Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~Tamara() = default;
    };

    // This is a free function, needs to be defined outside of the scope of the class
    // This function then is actually called by the int main() from the EntryPoint.h which starts the application
    Application* CreateApplication()
    {
        printf("Mashenka is a twin sister of my Tamara!");
        return new Tamara();
    }
}
