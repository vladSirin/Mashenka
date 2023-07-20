#pragma once
#include "Core.h"
#include "Events/Event.h"

namespace Mashenka
{
    class MASHENKA_API Application
    {
    public:
        Application();

        virtual ~Application();
        
        void Run();
    };

    // To be defined in Client
    Application* CreateApplication();
    
}

