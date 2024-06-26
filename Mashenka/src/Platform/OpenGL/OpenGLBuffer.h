﻿#pragma once
#include "Mashenka/Renderer/Buffer.h"

namespace Mashenka
{
    // OpenGLBuffer Class
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        // Constructor & Destructor
        // why constructor when we have Create()?
        // because we want to create a buffer when we create a vertex buffer
        // we don't want to create a vertex buffer and then create a buffer
        // we want to create a vertex buffer and a buffer at the same time
        // so we need a constructor
        // why constructor is not virtual?
        // because we don't want to create a vertex buffer and a buffer at the same time
        // we want to create a vertex buffer and a OpenGL vertex buffer at the same time
        // why we want to create them at the same time?
        // because we want to use the same function to create different vertex buffers
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        // Bind & Unbind
        void Bind() const override;
        void Unbind() const override;

        // set the data for the butter as the creation using only size provide nothing.
        virtual void SetData(const void* data, uint32_t size) override;

        // Get and Set the layout of the buffer
        virtual const BufferLayout& GetLayout() const override {return m_Layout; }
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

    private:
        // the id of the vertex buffer
        // what is the id used for?
        // when we want to use the vertex buffer, we need to bind it
        // we need to bind it by its id
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    // OpenGl Index Buffer Class
    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        // bind and unbind
        void Bind() const override;
        void Unbind() const override;

        // get the count of indices
        virtual uint32_t GetCount() const override { return m_Count; }

    private:
        // the number of indices
        uint32_t m_Count;
        // the id of the index buffer
        uint32_t m_RendererID;
    };
}
