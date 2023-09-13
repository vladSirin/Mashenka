#include "mkpch.h"
#include "OpenGLContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Mashenka
{
    Mashenka::OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        MK_CORE_ASSERT(windowHandle, "Window hanlde is null")
    }

    void Mashenka::OpenGLContext::Init()
    {
        /*
         * gladLoadGLLoader is a function provided by GLAD,
         * a loader-generator for OpenGL. It's used to load and initialize the OpenGL function pointers for the context
         * that's currently bound. In simpler terms, it sets up all the OpenGL functions so that
         * they can be used in your application.
         * glfwGetProcAddress is a function provided by GLFW that retrieves the address of an OpenGL function.
         * By casting it to GLADloadproc, you're telling GLAD to use this function to
         * load all the required OpenGL function addresses.
        */
        glfwMakeContextCurrent(m_WindowHandle); // Associate the context with current window, crucial step for next
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        MK_CORE_ASSERT(status, "Failed to initialize Glad!");
    }

    void Mashenka::OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}

