#include "mkpch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Mashenka
{
    /*
     * VertexBuffer
     */
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        // create a buffer
        glGenBuffers(1, &m_RendererID);
        // bind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        // allocate memory for the buffer
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        // delete the buffer
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        // bind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        // unbind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    /*
     * Index buffer
     */
    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    {
        // set the count
        m_Count = count;
        // create a buffer
        glGenBuffers(1, &m_RendererID);
        // bind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        // allocate memory for the buffer
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        // delete the buffer
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        // bind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        // unbind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

