#pragma once
#include "RendererAPI.h"
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
    class MASHENKA_API RenderCommand
    {
    public:
        // init
        inline static void Init() { s_RendererAPI->Init(); }
        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RendererAPI->SetViewport(x, y, width, height); }

        // static functions to call RendererAPI functions
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
        inline static void Clear() { s_RendererAPI->Clear(); }
        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

        

    private:
        static Scope<RendererAPI> s_RendererAPI;
    };
}

