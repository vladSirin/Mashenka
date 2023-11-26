#pragma once
#include "Mashenka/Renderer/RendererAPI.h"

namespace Mashenka
{
    class OpenGLRendererAPI : public Mashenka::RendererAPI
    {
    public:
        // init
        virtual void Init() override;
        // viewport
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        // override the virtual functions from RendererAPI
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void DrawIndexed(const std::shared_ptr<Mashenka::VertexArray>& vertexArray) override;
    
    
    };
}

