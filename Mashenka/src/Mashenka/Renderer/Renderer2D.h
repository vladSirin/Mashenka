#pragma once
#include "OrthographicCamera.h"

#include "Texture.h"

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
        static void DrawQuad(const glm::vec2& position, float angle, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, float angle, const glm::vec2& size, const glm::vec4& color);

        // texture rendering functions:
        static void DrawQuad(const glm::vec2& position, float angle, const glm::vec2& size, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, float angle, const glm::vec2& size, const Ref<Texture2D>& texture);
    
    };
}

