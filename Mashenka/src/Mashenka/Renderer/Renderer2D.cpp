#include "mkpch.h"
#include "Mashenka/Renderer/Renderer2D.h"

#include "Mashenka/Renderer/VertexArray.h"
#include "Mashenka/Renderer/Shader.h"
#include "Mashenka/Renderer/RenderCommand.h"
// #include "Platform/OpenGL/OpenGLShader.h", but we can't include it here because it will cause a circular dependency
#include <glm/gtc/matrix_transform.hpp> // for glm::mat4

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"

namespace Mashenka
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float textureIndex; // use for indexing textures
        float TilingFactor;
    };

    struct Renderer2DData
    {
        // keep consistency for the struct and data as they are both static
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; //TODO: RenderCaps

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots; // slots for binding texture
        uint32_t TextureSlotIndex = 1; // 0 = White Texture

        // Adding Vertex Position for transform
        glm::vec4 QuadVertexPositions[4];

        // Saving the stats into the Renderer Data itself
        Renderer2D::Statistics Stats;
    };


    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        MK_PROFILE_FUNCTION(); // Profiling

        s_Data.QuadVertexArray = VertexArray::Create(); // Create vertex array
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)); // Create vertex buffer

        // Set vertex buffer layout
        s_Data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"},
            {ShaderDataType::Float, "a_textureIndex"}, // adding the textureIndex in the layout
            {ShaderDataType::Float, "a_TilingFactor"},
        });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer); // Add vertex buffer to vertex array
        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices]; // Create vertex buffer base

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices]; // Create index buffer

        uint32_t offset = 0; // Offset for index buffer

        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB); // Set index buffer to vertex array
        delete[] quadIndices; // Delete index buffer

        s_Data.WhiteTexture = Texture2D::Create(1, 1); // Create white texture
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t)); // Set white texture data

        // Create samplers later will be upload to shader
        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl"); // Create texture shader
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
        // @WARNING: Make sure the name is the same with the shader file, or it will not work correctly and no error

        // Set the first slot to be whitetexture
        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        // Define the vertices positions of a unit 1 quad
        s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    }

    void Renderer2D::Shutdown()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        MK_PROFILE_FUNCTION(); // Profiling
        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

        // reset index count and vertex buffer pointer
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        // Setting the texture index back to 1 as 0 is white, binding will be restarted every drawcall
        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        MK_PROFILE_FUNCTION(); // Profiling

        uint32_t dataSize = (uint32_t)((uintptr_t)s_Data.QuadVertexBufferPtr - (uintptr_t)s_Data.QuadVertexBufferBase);
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        // Nothing to draw, avoiding drawing an empty vertex array
        if (s_Data.QuadIndexCount == 0)
            return;
        
        // bind textures for render
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
        {
            s_Data.TextureSlots[i]->Bind(i);
        }
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        s_Data.Stats.DrawCalls++; //adding up drawcalls count every time
    }


    // This function is called when the max indices is reached for the current drawcall
    void Renderer2D::FlushAndReset()
    {
        EndScene();

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }
    

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        MK_PROFILE_FUNCTION(); // Profiling

        //MK_CORE_INFO("Renderer2D::DrawQuad(position: {0}, size: {1},  {2})", glm::to_string(position), glm::to_string(size), glm::to_string(color));
        constexpr float textureIndex = 0.0f; // using white texture as it's a color Drawing
        constexpr float tilingFactor = 1.0f; // no tiling for pure color
        const glm::vec2 texCoord = {0.0f, 0.0f};
        
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        // Matrix, to comment and explain how does it work and why so
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(0.0f), {0.0f, 0.0f, 1.0f}) * glm::scale(
            glm::mat4(1.0f), {size.x, size.y, 1.0f});

        SetupQaudVertexBuffer(transform, color, textureIndex, tilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                              float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                              float tilingFactor, const glm::vec4& tintColor)
    {
        MK_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        const glm::vec4 color = tintColor;
        float textureIndex = 0.0f;
        bool textureFound = false;

        // Check if the texture is already pointed by any slot
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = float(i); // use it if true
                textureFound = true;
                break;
            }
        }

        // Set the current Index to the new texture
        if (!textureFound)
        {
            if (s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
                return;

            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++; // Move to next slot
        }

        // Matrix, to comment and explain how does it work and why so
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(0.0f), {0.0f, 0.0f, 1.0f}) * glm::scale(
            glm::mat4(1.0f), {size.x, size.y, 1.0f});

        SetupQaudVertexBuffer(transform, color, textureIndex, tilingFactor);

        
#if OLD_PATH
        MK_PROFILE_FUNCTION(); // Profiling
        s_Data.TextureShader->SetFloat4("u_Color", tintColor);
        s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
        texture->Bind(); // bind the texture

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(
            glm::mat4(1.0f), glm::vec3(size.x, `size.y, 1.0f));
        s_Data.TextureShader->SetMat4("u_Transform", transform);

        s_Data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const glm::vec4& color)
    {
        MK_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        constexpr float textureIndex = 0.0f; // using white texture as it's a color Drawing
        constexpr float tilingFactor = 1.0f; // no tiling for pure color

        // Transform matrix, typically calculated as Translate * Rotation * Scale
        // The order of the matrix operations matter!
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(
            glm::mat4(1.0f), {size.x, size.y, 1.0f});

        SetupQaudVertexBuffer(transform, color, textureIndex, tilingFactor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                     const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        MK_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            FlushAndReset();

        const glm::vec4 color = tintColor;
        const glm::vec2 texCoord = {0.0f, 0.0f};
        float textureIndex = 0.0f;
        bool textureFound = false;

        // Check if the texture is already pointed by any slot
        for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i)
        {
            if (*s_Data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = float(i); // use it if true
                textureFound = true;
                break;
            }
        }

        // Set the current Index to the new texture
        if (!textureFound)
        {
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                FlushAndReset();

            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
            s_Data.TextureSlotIndex++; // Move to next slot
        }

        // Matrix, to comment and explain how does it work and why so
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
            glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) * glm::scale(
            glm::mat4(1.0f), {size.x, size.y, 1.0f});

        SetupQaudVertexBuffer(transform, color, textureIndex, tilingFactor);
    }

    void Renderer2D::SetupQaudVertexBuffer(glm::mat4 transform, glm::vec4 color, float textureIndex,
                                        float tilingFactor)
    {
        constexpr size_t quadVertexCount = 4;
        const glm::vec2 textureCoord[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

        for (size_t i = 0; i < quadVertexCount; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoord[i];
            s_Data.QuadVertexBufferPtr->textureIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;

        //adding up quad stat data
        s_Data.Stats.QuadCount++;
    }


    // Reset stats when the rendering starts every frame
    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }


}
