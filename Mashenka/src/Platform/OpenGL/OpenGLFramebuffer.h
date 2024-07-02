#pragma once
#include "glm/ext/scalar_uint_sized.hpp"
#include "Mashenka/Renderer/Framebuffer.h"

namespace Mashenka
{
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        ~OpenGLFramebuffer() override;

        // clean up the framebuffer and initialize it
        void Invalidate();

        void Bind() override;
        void Unbind() override;

        void Resize(uint32_t width, uint32_t height) override;

        uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
        const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
        
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment= 0, m_DepthAttachment = 0;
        FramebufferSpecification m_Specification;
    };
}
