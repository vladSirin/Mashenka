﻿#include "mkpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "glad/glad.h"

namespace Mashenka
{
    // This is the dynamic version of the creation of VB
    // By only allocate the size needed but not provide the actual data
    // The data will be provided later by SetData
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        MK_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

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
        
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
		// Normally here GL_ELEMENT_ARRAY_BUFFER should be used
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        // allocate memory for the buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);  // NOLINT(bugprone-narrowing-conversions)
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

