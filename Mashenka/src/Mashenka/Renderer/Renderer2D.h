#pragma once
#include "Mashenka/Renderer/OrthographicCamera.h"
#include "Mashenka/Renderer/Texture.h"

namespace Mashenka
{
    // This class will be used to render 2D objects
    class Renderer2D
    {
    public:
        // basic functions:
        static void Init();
        static void Shutdown();
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        //primitive rendering functions:
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

        // texture rendering functions:
        // using tintColor for texture blending and other flexibilities
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture,
                             float tilingFactor = 1.0f,
                             const glm::vec4& tintColor = glm::vec4(1.0f));

        // Draw rotated Quad
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);

        // Draw rotated quad with textures
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D>& texture, float tilingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D>& texture, float tilingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));
    };
}
