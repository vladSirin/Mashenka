#pragma once

#include "Mashenka/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Mashenka
{
    // the OpenGL implementation of Graphic Context
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle; // Window is needed for OpenGL initialization and operations like swap buffer
    };
    
}
