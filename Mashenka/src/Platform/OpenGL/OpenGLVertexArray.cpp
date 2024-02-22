#include "mkpch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Mashenka
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4: return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4: return GL_INT;
        case ShaderDataType::Bool: return GL_BOOL;
        case ShaderDataType::None: break;
        }

        MK_CORE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }

    // Constructor
    OpenGLVertexArray::OpenGLVertexArray()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        glCreateVertexArrays(1, &m_RendererID);
    }

    // Destructor
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        glDeleteVertexArrays(1, &m_RendererID);
    }

    // Bind the vertex array
    void OpenGLVertexArray::Bind() const
    {
        MK_PROFILE_FUNCTION(); // Profiling
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        MK_PROFILE_FUNCTION(); // Profiling
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // set index buffer for the opengl vertex array
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // add vertex buffer into the vertex array
        MK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")
        
        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        // initialize a index and inject the vertex buffer into the vertex array
        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            // enable the vertex attribute array
            glEnableVertexAttribArray(index + m_VertexBufferIndex);
            // set the vertex attribute pointer
            glVertexAttribPointer(
                index + m_VertexBufferIndex,
                static_cast<GLint>(element.GetComponentCount()),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void*>(element.
                    Offset) // the offset of the element in the buffer. removing intptr_t because it is not defined in this scope
                // intptr_t is a signed integer type with the property that any valid pointer to void can be converted to this type, then converted back to pointer to void, and the result will compare equal to the original pointer.
                // intptr_t is used to represent the difference between two pointers, thus the size of intptr_t is the same as the size of a pointer.
            );
            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }
}
