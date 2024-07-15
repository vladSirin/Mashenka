#include "mkpch.h"
#include "Platform/OpenGl/OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Mashenka
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
        MK_CORE_ASSERT(windowHandle, "Window hanlde is null")
    }

    void OpenGLContext::Init()
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
        MK_PROFILE_FUNCTION(); // Profiling
        glfwMakeContextCurrent(m_WindowHandle); // Associate the context with current window, crucial step for next
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        MK_CORE_ASSERT(status, "Failed to initialize Glad!")

        // Logging the rendering info
        MK_CORE_INFO("OpenGL Info:");
        MK_CORE_INFO(" Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR))); //eg: Intel
        MK_CORE_INFO(" Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER))); //eg: RTX 4060
        MK_CORE_INFO(" Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION))); //eg: OPENGL Version

        // Enable OpenGL Debug Context, the version should be major over 4 or major is 4 but minor is above 0.5
        MK_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5),
                       "Mashenka requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        glfwSwapBuffers(m_WindowHandle);
    }
}
