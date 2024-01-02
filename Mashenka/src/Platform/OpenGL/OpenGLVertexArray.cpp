#include "mkpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Mashenka
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case Mashenka::ShaderDataType::Float:
        case Mashenka::ShaderDataType::Float2:
        case Mashenka::ShaderDataType::Float3:
        case Mashenka::ShaderDataType::Float4:
        case Mashenka::ShaderDataType::Mat3:
        case Mashenka::ShaderDataType::Mat4: return GL_FLOAT;
        case Mashenka::ShaderDataType::Int:
        case Mashenka::ShaderDataType::Int2:
        case Mashenka::ShaderDataType::Int3:
        case Mashenka::ShaderDataType::Int4: return GL_INT;
        case Mashenka::ShaderDataType::Bool: return GL_BOOL;
        case Mashenka::ShaderDataType::None: break;
        }

        MK_CORE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }

    Mashenka::OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    Mashenka::OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void Mashenka::OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void Mashenka::OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void Mashenka::OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        // set index buffer for the opengl vertex array
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();
        m_IndexBuffer = indexBuffer;
    }

    void Mashenka::OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
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
