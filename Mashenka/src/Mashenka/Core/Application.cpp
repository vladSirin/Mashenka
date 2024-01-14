#include "mkpch.h"
#include "Mashenka/Core/Application.h"
#include "Mashenka/Core/Log.h"
#include "Mashenka/Core/Input.h"
#include "Mashenka/Core/TimeStep.h"
#include "Mashenka/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Mashenka
{

    // initialize the singleton instance of application as null
    Application* Application::s_Instance = nullptr;

    // this is the constructor of the application class
    Application::Application()
    {
        // ==================== Make sure Singleton and setup Window ====================
        // Make sure there is only one instance
        MK_CORE_ASSERT(!s_Instance, "Application Alreay Exists!")
        s_Instance = this;

        // Create the window
        m_Window = Window::Create();
        m_Window->SetVSync(false);


        // using bind to call the member function of application when needed in Window layer
        // set the application OnEvent function to the window layer as a callback
        m_Window->SetEventCallback(MK_BIND_EVENT_FN(Application::OnEvent));

        // Create and assign the ImGuiLayer, Push it into the stack
        // This was done by sandbox app, which is not ideal as it should be part of the engine app
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        // ==================== Initialize the Renderer ====================
        // Initialize the Renderer
        Renderer::Init();
        
    }

    Application::~Application()
    {
        Renderer::Shutdown();
    }

    // Define the OnEvent function, this works similar to all OnEvent functions
    // Basically it creates a dispatcher and bind the function to it
    void Application::OnEvent(Event& e)
    {
        // Create a dispatcher
        EventDispatcher dispatcher(e);

        // Dispatch (which takes a template func as input), will check if the input matches the type
        // If so, the bound event will be called (which is the Application.OnWindowClose functions)
        dispatcher.Dispatch<WindowCloseEvent>(MK_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(MK_BIND_EVENT_FN(Application::OnWindowResize));

        /*
         * Explain the following code:
         * The following code is to go through all the layers in the stack and call the OnEvent function
         * This is to make sure that all the layers can handle the event
         * The event will be handled if the event is handled by any of the layers
         * This is done by going through the layers from the end to the beginning
         * This is because the layers are ordered from bottom to top
         * The layers at the bottom are the ones that are closer to the engine
         * The layers at the top are the ones that are closer to the client
         * The layers at the bottom should be able to handle the event first
         * If the event is handled, then the event will not be passed to the layers above
         * This is done by checking the event handled flag
         */
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }

        MK_CORE_TRACE("OnEvent {0}", e);
    }

    // The on attach now is called in LayerStack when Push, which it should be.
    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
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
            // Calculate the Delta Time based on the TimeStep
            float time = (float)glfwGetTime(); // Platform::GetTime()
            TimeStep timeStep = time - m_LastFrameTime;
            m_LastFrameTime = time;


            // Poll Input, this is the polling of the input system
            Input::Poll();

            // Go through all the layers, as each layer can handle its own update
            if (!m_Minimized)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timeStep); // Update the needed info
            }

            // Initialize the ImGui frame, prepare for the rendering, context and input
            m_ImGuiLayer->Begin();

            // Go through all the layers, as each layer can handle its own ImGui component
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender(); // Render the needed info

            // finalize the rendering for the current frame, wraps up tasks like draw data
            m_ImGuiLayer->End();

            // Render the next frame and poll the glfw events
            m_Window->OnUpdate();
        }
    }

    // Definition of the OnWindowClose function
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        // Check if the window is minimized
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        // Set the minimized flag to false
        m_Minimized = false;
        // Set the viewport
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
        return false;
    }
}
