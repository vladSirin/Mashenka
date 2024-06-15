#pragma once
#include "Mashenka/Renderer/RendererAPI.h"
#include "VertexArray.h"
#include "glm/vec4.hpp"

namespace Mashenka
{
    /*
     * RenderCommand Class
     * This is static class served as a wrapper for RendererAPI class as it will be platform-specific
     * RendererAPI is an abstract class, so we can't create an instance of it
     * RendererAPI will be implemented in the platform-specific code
     */
    class RenderCommand
    {
    public:
        // init
        inline static void Init() { s_RendererAPI->Init(); }

        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        // static functions to call RendererAPI functions
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
        inline static void Clear() { s_RendererAPI->Clear(); }
        /**
         * Draw the indexed vertices using the RendererAPI.
         *
         * @param vertexArray The vertex array containing the vertices to draw.
         * @param count The number of indices to draw (default is 0 for all).
         */
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
        {
            s_RendererAPI->DrawIndexed(vertexArray, count);
        }

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}
