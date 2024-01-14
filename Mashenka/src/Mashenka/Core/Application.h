#pragma once
#include "Mashenka/Core/Core.h"
#include "Mashenka/Core/Window.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Mashenka/ImGui/ImGuiLayer.h"

/*
 * implement a form of the Singleton pattern for the Application class.
 * The Singleton pattern ensures that a class has only one instance and provides a global point of access to that instance.
 */
namespace Mashenka
{
    class Application
    {
    public:
        Application();

        // virtual destructor to make sure the derived class destructor is called
        // explain this: https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        

        //Using a static function to get the sole instance of the application
        inline Window& GetWindow() const {return *m_Window;}
        inline static Application& Get() {return *s_Instance;}

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer; //adding ImGuiLayer variable for the application as it should be handled inside the engine

        // declare a static global single instance to access
        static Application* s_Instance;

        // define the last frame time
        float m_LastFrameTime = 0.0f;
    };

    // To be defined in Client
    Application* CreateApplication();
}
