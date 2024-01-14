#include "mkpch.h"
#include "Mashenka/Renderer/GraphicsContext.h"

#include "Mashenka/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Mashenka
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        MK_CORE_ASSERT(false, "Unknown RendererAPI!")
            return nullptr;
    }
}

