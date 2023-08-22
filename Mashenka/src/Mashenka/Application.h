#pragma once
#include "Core.h"
#include "Window.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "LayerStack.h"

/*
 * implement a form of the Singleton pattern for the Application class.
 * The Singleton pattern ensures that a class has only one instance and provides a global point of access to that instance.
 */
namespace Mashenka
{
    class MASHENKA_API Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        //Using a static function to get the sole instance of the application
        inline Window& GetWindow() {return *m_Window;}
        inline static Application& Get() {return *s_Instance;}

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;

        // declare a static global single instance to access
        static Application* s_Instance;
    };

    // To be defined in Client
    Application* CreateApplication();
}
