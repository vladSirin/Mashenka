#include "mkpch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Mashenka
{
    // Shader create functions
    Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(vertexSrc, fragmentSrc);
        }
        return nullptr;
    }

    Shader* Shader::Create(const std::string& filepath)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLShader(filepath);
        }
        return nullptr;
    }
}
