#include "mkpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Mashenka
{
    Mashenka::VertexBuffer* Mashenka::VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        }
    }

    Mashenka::IndexBuffer* Mashenka::IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        // Factory Method for different renderer API
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        };
    }
}


