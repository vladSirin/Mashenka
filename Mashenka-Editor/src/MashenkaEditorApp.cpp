// Engine: Mashenka Game Engine
#include "Mashenka.h"
#include "Mashenka/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Mashenka
{
    class MashenkaEditor : public Application
    {
    public:
        MashenkaEditor() : Application("Mashenka Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~MashenkaEditor() = default;
    };

    // This is a free function, needs to be defined outside of the scope of the class
    // This function then is actually called by the int main() from the EntryPoint.h which starts the application
    Application* CreateApplication()
    {
        printf("MashenkaEditor is now on!");
        return new MashenkaEditor();
    }
}
