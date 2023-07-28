#include "mkpch.h"
#include "WindowsWindow.h"

namespace Mashenka
{
    // Check if glfw is initialized, shared by all instance to make sure glfw is initialized once only
    // As glfwinit() should only be called once during the execution of a program
    static bool s_GLFWInitialized = false;

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    // Using Init and Shutdown to organize and potentially reuse code when other ways to construct or destruct exists
    // It is also friendly to error handling as constructors and destructors are not allowed to throw exceptions or error codes
    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        MK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            //TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            MK_CORE_ASSERT(success, "Could not initialize GLFW!");

            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }
    
    void WindowsWindow::OnUpdate()
    {
        // Checks for any pending events and handle them, if not app or window will not respond
        glfwPollEvents();

        // Swap the back buffer with the front buffer as openGL normally render to off-screen buffer to avoid flickering
        glfwSwapBuffers(m_Window);
    }

    // Setup Vertical Sync for the GPU
    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
        {
            glfwSwapInterval(0);
        }

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }
}
