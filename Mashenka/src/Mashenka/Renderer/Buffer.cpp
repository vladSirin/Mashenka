#include "mkpch.h"
#include "Mashenka/Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Mashenka/Renderer/Renderer.h"

namespace Mashenka
{
    /**
     * Create a new VertexBuffer based on the RendererAPI in use.
     *
     * @param size The size of the VertexBuffer.
     *
     * @return A reference to the created VertexBuffer.
     *
     * @throws Assertion error if RendererAPI is None or unknown.
     */
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(size);
        }

        MK_CORE_ASSERT(false, "Unknown RendererAPI!")

        return nullptr;
    }

    // Factory Method for different renderer API
    // Using create and raw ptr because we don't want to expose the implementation of the derived class
    // We only want to expose the interface of the base class
    // The derived class is only used internally, so we don't need to worry about memory leak
    // The derived class is deleted when the base class is deleted
    // This is called polymorphism
    Ref<VertexBuffer> Mashenka::VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }
        return nullptr;
    }

    Ref<IndexBuffer> Mashenka::IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        // Factory Method for different renderer API
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLIndexBuffer>(indices, count);
        }
        return nullptr;
    }
}
