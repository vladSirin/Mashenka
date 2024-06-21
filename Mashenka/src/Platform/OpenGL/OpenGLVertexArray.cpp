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

    /* This function is used to bind and config the data for the vertex buffer object
     * that is used for the current renderer*/
    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        MK_PROFILE_FUNCTION(); // Profiling

        // Ensure the buffer has a layout
        MK_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!")

        // binds renderer to make sure it is the current vertex array
        glBindVertexArray(m_RendererID);
        // binds the VBO
        vertexBuffer->Bind();

        // processing vertex buffer layout
        uint32_t index = 0;
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
            // for scalar and vector types
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(m_VertexBufferIndex); //enable vertex attribute array

                    /* Specifies the layout of the vertex attribute data.
                     * 'element.GetComponentCount()' returns the num of components (eg: 1 for float, 3 for float3)
                     * 'ShaderDataTypeToOpenGLBaseType(element.Type)' converts the type to openGL type
                     * 'element.Normalized' specifics whether data should be normalized
                     * @layout.GetStride(), specifies the byte offset between consecutive vertex attributions
                     * @element.Offset specifies the offset of the first component.
                     */
                    glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
                                          (const void*)element.Offset);
                    m_VertexBufferIndex++; // increment attribute index
                    break;
                }

            // for matrix types
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4:
                {
                    // Matrices are handled as multiple attribute pointers, one for each column of the matrix
                    uint8_t count = element.GetComponentCount();

                    // using loop to process data
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_VertexBufferIndex);
                        glVertexAttribPointer(m_VertexBufferIndex, count,
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(sizeof(float) * count * i));
                        glVertexAttribDivisor(m_VertexBufferIndex, 1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
            default:
                MK_CORE_ASSERT(false, "Unknow ShaderDataType!") // error handling 
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }
}
