#pragma once

namespace Mashenka
{
    // the type of the shader data
    // the size of the data type is used to calculate the offset of the data
    // for example, if the first element is a float, the offset is 0
    // if the second element is a float2, the offset is 4
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    // get the size of the shader data type
    static uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float: return 4;
        case ShaderDataType::Float2: return 4 * 2;
        case ShaderDataType::Bool: return 1;
        case ShaderDataType::Float3: return 4 * 3;
        case ShaderDataType::Float4: return 4 * 4;
        case ShaderDataType::Int: return 4;
        case ShaderDataType::Int2: return 4 * 2;
        case ShaderDataType::Int3: return 4 * 3;
        case ShaderDataType::Int4: return 4 * 4;
        case ShaderDataType::Mat3: return 4 * 3 * 3;
        case ShaderDataType::Mat4: return 4 * 4 * 4;
        case ShaderDataType::None: return 0;
        }

        MK_CORE_ASSERT(false, "unknown shader data type")
        return 0;
    }

    // the layout of the buffer
    // the buffer is a list of elements, each element has a name, a type, a size and an offset, and whether it is normalized
    // the offset is the offset of the element in the buffer, the size is the size of the element, the type is the type of the element
    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        uint32_t Size;
        size_t Offset;
        //size_t is a type that can store the size of the largest object in the current environment, uint32_t is not enough because the size of the object may be larger than 4GB
        bool Normalized;

        // constructor, destructor and default constructor, the default constructor is used to create an empty buffer element
        // i.e. without any parameter setup
        BufferElement() = default;

        // constructor
        BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
            : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
        {
        }

        // get the count of the component, for example, float3 has 3 components, float4 has 4 components
        uint32_t GetComponentCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Mat3: return 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::None: break;
            }

            MK_CORE_ASSERT(false, "unknown shader data type")
            return 0;
        }
    };

    // the layout of the buffer
    // the buffer is a list of elements, each element has a name, a type, a size and an offset, and whether it is normalized
    // the offset is the offset of the element in the buffer, the size is the size of the element, the type is the type of the element
    // the stride is the size of the buffer, the elements are stored in the buffer one by one.
    class BufferLayout
    {
    public:
        // constructor, destructor and default constructor, the default constructor is used to create an empty buffer layout
        BufferLayout() = default;

        // constructor
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements)
        {
            CalculateOffsetAndStride();
        }

        // get stride and elements
        // stride is the size of the buffer, the elements are stored in the buffer one by one.
        inline uint32_t GetStride() const { return m_Stride; }

        // elements is the list of elements
        inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

        // begin and end, used to iterate the elements
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

        // begin and end, used to iterate the elements, const version for immutable situations
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        // calculate the offset and stride of the buffer
        void CalculateOffsetAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    // base vertex buffer class
    // virtual class is used to make sure the derived class is deleted when the base class is deleted
    class VertexBuffer
    {
    public:
        // constructor and destructor
        // virtual is used to make sure the destructor is called when the derived class is deleted
        // this is because the derived class is deleted first, then the base class
        // if the destructor is not virtual, the derived class destructor will not be called
        // and the derived class will not be deleted
        virtual ~VertexBuffer() =default;

        // bind and unbind
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // set the data of the buffer
        virtual void SetData(const void* data, uint32_t size) = 0;

        // get the layout of the buffer
        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        // create a new vertex buffer
        // the size is the size of the vertices
        static Ref<VertexBuffer> Create(uint32_t size); // the size is the size of the vertices
        static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
    };

    // base index buffer class
    // currently Mashenka only supports 32-bit index buffers
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() =default;

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
        static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };
}
