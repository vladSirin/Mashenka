#include "mkpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "glad/glad.h"

namespace Mashenka
{
    /*
     * VertexBuffer
     */
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // create a buffer
        glGenBuffers(1, &m_RendererID);
        // bind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        // allocate memory for the buffer
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // delete the buffer
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // bind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // unbind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    /*
     * Index buffer
     */
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count)
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // create a buffer
        glGenBuffers(1, &m_RendererID);
        // bind the index buffer TODO: why use GL_ELEMENT_ARRAY_BUFFER? why not GL_ARRAY_BUFFER? what is the difference?
        // GL_ARRAY_BUFFER is used for vertex buffer, GL_ELEMENT_ARRAY_BUFFER is used for index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        // allocate memory for the buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);  // NOLINT(bugprone-narrowing-conversions)
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // delete the buffer
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // bind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // unbind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

