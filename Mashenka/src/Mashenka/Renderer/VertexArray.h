#pragma once

#include <memory>
#include "Mashenka/Renderer/Buffer.h"

namespace Mashenka
{
    /*
     * VertexArray is a wrapper class for VertexBuffer and IndexBuffer, OpenGL calls it Vertex Array Object
     * This class is a OpenGL specific class
     * This class is used to bind the vertex buffer and index buffer
     * However, it is not responsible for the creation of the buffers
     * Why? Because the buffers are created by the Renderer API
     */
    class VertexArray
    {
    public:
        virtual ~VertexArray()
        {
        }

        // Bind and Unbind the vertex array, which is the vertex buffer and index buffer
        // The vertex array is the vertex buffer and index buffer, which are the buffers that contain the vertices to be drawn by the GPU
        virtual void Bind() const =0;
        virtual void Unbind() const =0;

        // Add the vertex buffer and index buffer to the vertex array
        // Why not just bind the vertex buffer and index buffer?
        // OpenGL requires that the vertex buffer and index buffer be bound to the vertex array
        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) =0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) =0;

        // Get the vertex buffer and index buffer from the vertex array
        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const =0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const =0;

        static Ref<VertexArray> Create();
    };
}
