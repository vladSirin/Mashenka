#include "mkpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Mashenka
{
    //Create func
    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        //switch the api based on the RendererAPI
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL: return std::make_shared<OpenGLTexture2D>(path);
        }

        //if the api is not supported
        MK_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        //switch the api based on the RendererAPI
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None: MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
        }

        MK_CORE_ASSERT(false, "Unknown RendererAPI!")
        return nullptr;
    }
}
