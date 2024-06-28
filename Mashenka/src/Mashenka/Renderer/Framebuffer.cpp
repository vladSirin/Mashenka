﻿#include "mkpch.h"
#include "Framebuffer.h"
#include "Mashenka/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Mashenka
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!") return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLFramebuffer>(spec);
        }

        MK_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }

}
