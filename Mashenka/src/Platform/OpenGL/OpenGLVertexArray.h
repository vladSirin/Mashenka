#pragma once
#include "Mashenka/Renderer/VertexArray.h"

namespace Mashenka
{
    // This is the OpenGL specific implementation of the VertexArray class
    class OpenGLVertexArray : public VertexArray
    {
    public:

        // Constructor and Destructor
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        // Bind and Unbind the vertex array, which is the vertex buffer and index buffer
        void Bind() const override;
        void Unbind() const override;

        // Add the vertex buffer and index buffer to the vertex array
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
        void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;

        // Get the vertex buffer and index buffer from the vertex array
        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override {return  m_VertexBuffers; }
        const Ref<IndexBuffer>& GetIndexBuffer() const override {return m_IndexBuffer; }

    private:
        // The unique identifier of the vertex array
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0; // offset of the vertex buffer index
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}
