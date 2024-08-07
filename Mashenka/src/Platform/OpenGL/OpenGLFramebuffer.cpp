﻿#include "mkpch.h"
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace Mashenka
{
    static const uint32_t s_MaxFrameBufferSize = 8192; // warning for buffer size
    
    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec)
    {
        Invalidate();
    }

    /* Make sure all OpenGL resources associated are released before the framebuffer is destroyed */
    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID); // n is the number of framebuffers to be deleted

        // delete the components of the framebuffer renderer too to make sure it's cleaned
        glDeleteTextures(1, &m_ColorAttachment); 
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }

        // Create framebuffer
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); // make it the current framebuffer

        // Creating and configuring the color attachment texture;
        // using a DSA API glTextureStorage2D
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minification
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnification for smooth
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               m_ColorAttachment,
                               0); // attaches the texture to the framebuffer's color attachment point

        // Creating and configuring the depth attachment texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH24_STENCIL8,
                           m_Specification.Width, m_Specification.Height); // allocates immutable storage for texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                               m_DepthAttachment,
                               0); // attaches the texture to the framebuffer's depth and stencil attachment point

        GLenum bBufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        MK_CORE_ASSERT(bBufferComplete, "Framebuffer is incomplete")

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width ==0 || height ==0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
        {
            MK_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
            return;
        }
        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }
}
