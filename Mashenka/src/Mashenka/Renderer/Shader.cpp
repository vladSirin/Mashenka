#include "mkpch.h"
#include "Shader.h"

#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Mashenka
{
    // Shader create functions
    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }
        return nullptr;
    }

    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            MK_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(filepath);
        }
        return nullptr;
    }

    // Shader Library Class
    void ShaderLibrary::add(const std::string& name, const std::shared_ptr<Shader>& shader)
    {
        // add shader into the map
        MK_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_Shaders[name] = shader;
    }

    void ShaderLibrary::add(const std::shared_ptr<Shader>& shader)
    {
        // add shader into map
        auto& name = shader->GetName();
        add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        // load shader from file path
        auto shader = Shader::Create(filepath);
        add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        // load shader from file path
        auto shader = Shader::Create(filepath);
        add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        // get shader from the map
        MK_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_Shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        // check if the shader exists in the map
        return m_Shaders.find(name) != m_Shaders.end();
    }
}
