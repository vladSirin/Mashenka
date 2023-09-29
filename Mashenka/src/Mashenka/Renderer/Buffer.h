#pragma once

namespace Mashenka
{
    // base vertex buffer class
    class VertexBuffer
    {
    public:
        // constructor and destructor
        // virtual is used to make sure the destructor is called when the derived class is deleted
        // this is because the derived class is deleted first, then the base class
        // if the destructor is not virtual, the derived class destructor will not be called
        // and the derived class will not be deleted
        virtual ~VertexBuffer(){}

        // bind and unbind
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // create a new vertex buffer
        // the size is the size of the vertices
        static VertexBuffer* Create(float* vertices, uint32_t size);
    };

    // base index buffer class
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer(){}

        // bind and unbind
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // get the count of indices
        virtual uint32_t GetCount() const = 0;

        // create a new index buffer
        // the count is the number of indices
        // the indices is the array of indices
        // why not use a normal constructor?
        // because we want to use the same function to create different index buffers
        // for example, OpenGL uses unsigned int as indices, but DirectX uses unsigned short
        // so we need to use different functions to create different index buffers
        // this is called a static factory function
        static IndexBuffer* Create(uint32_t* indices, uint32_t count);
    };
    
    
}

