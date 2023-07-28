#include "mkpch.h"
#include "Application.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "Mashenka/Log.h"
#include "GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Mashenka
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(WindowsWindow::Create());
    }

    Application::~Application()
    {
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
            glClearColor(1,0.42,0.5,0.5);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}
