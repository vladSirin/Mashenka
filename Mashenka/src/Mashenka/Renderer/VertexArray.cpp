#include "mkpch.h"
#include "Mashenka/Renderer/VertexArray.h"
#include "Mashenka/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mashenka
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
        }

        MK_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }
}
