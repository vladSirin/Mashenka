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

        // static functions to call RendererAPI functions
        inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }
        inline static void Clear() { s_RendererAPI->Clear(); }
        inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) { s_RendererAPI->DrawIndexed(vertexArray); }

        

    private:
        static RendererAPI* s_RendererAPI;
    };
}

