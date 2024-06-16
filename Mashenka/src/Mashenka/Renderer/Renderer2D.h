﻿#pragma once
#include "Mashenka/Renderer/OrthographicCamera.h"
#include "Mashenka/Renderer/Texture.h"

namespace Mashenka
{
    // This class will be used to render 2D objects including:
    /*
     * Initializing and shutting down the 2d renderer
     * Managing the rendering pipeline for 2d objects
     * Providing functions to draw 2d primitives(quads) and textured quads
     * Handling transformations and scene setup
     */
    class Renderer2D
    {
    public:
        // Init and shutdown
        static void Init();     // Sets up the vertex array, vertex buffer, index buffer, shaders and default white texture
        static void Shutdown(); // cleans up any dynamic allocated memory

        // Scene manage
        static void BeginScene(const OrthographicCamera& camera); // Prepares the renderer for a new scene by setting the view-projection matrix from the camera
        static void EndScene(); // Finalizes the scene, updates the vertex buffer with the new data, and flushes the draw calls
        static void Flush();

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

        // Draw rotated Quad, rotation in degree
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const glm::vec4& color);

        // Draw rotated quad with textures, rotation in degree
        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D>& texture, float tilingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation,
                                    const Ref<Texture2D>& texture, float tilingFactor = 1.0f,
                                    const glm::vec4& tintColor = glm::vec4(1.0f));

    private:
        // Submit data to Renderer2D data
        static void SubmitRendererData(glm::mat4 transform, glm::vec4 color, float textureIndex=0.0f, float tilingFactor=1.0f);
    };
}
