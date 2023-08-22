#include "mkpch.h"
#include "Application.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "Mashenka/Log.h"
#include <glad/glad.h>
#include "Platform/Windows/WindowsWindow.h"

namespace Mashenka
{
    /*The resulting callable object can be called with one argument,
     *and when it is called, it will invoke the x member function of the Application object,
     *passing along the given argument.*/
    #define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    // initialize the singleton instance of application as null
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        // Make sure there is only one instance
        MK_CORE_ASSERT(!s_Instance, "Application Alreay Exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(WindowsWindow::Create());

        // using bind to call the member function of application when needed in Window layer
        // set the application OnEvent function to the window layer as a callback
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application()
    {
    }

    // Define the OnEvent function
    void Application::OnEvent(Event& e)
    {
        // Create a dispatcher
        EventDispatcher dispatcher(e);

        // Dispatch (which takes a template func as input), will check if the input matches the type
        // If so, the bound event will be called (which is the Application.OnWindowClose functions)
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        /*
         * The purpose of this loop seems to be to process an event e in reverse order of the layers in the m_LayerStack
         * until one of the layers handles the event. Once the event is handled, the loop stops,
         * ensuring that layers earlier in the stack don't process an already-handled event.
         * This is a common pattern in event-driven systems
         * where events can be consumed by layers or components in a specific order of priority.
         */
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }

        MK_CORE_TRACE("{0}", e);
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }


    /*
     * 4. **Running the Application**:
     * The application is run by calling the `run` function on the application instance.
     * This initiates a while loop that keeps the application running.
     */
    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(1, 0.42, 0.5, 0.5);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    // Definition of the OnWindowClose function
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
