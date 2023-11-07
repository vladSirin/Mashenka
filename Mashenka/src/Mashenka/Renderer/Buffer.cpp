#include "mkpch.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Mashenka
{
    // Factory Method for different renderer API
    // Using create and raw ptr because we don't want to expose the implementation of the derived class
    // We only want to expose the interface of the base class
    // The derived class is only used internally, so we don't need to worry about memory leak
    // The derived class is deleted when the base class is deleted
    // This is called polymorphism
    Mashenka::VertexBuffer* Mashenka::VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        }
        return nullptr;
    }

    Mashenka::IndexBuffer* Mashenka::IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        // Factory Method for different renderer API
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        }
        return nullptr;
    }
}


