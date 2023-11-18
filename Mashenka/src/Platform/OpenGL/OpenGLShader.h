#pragma once
#include "Mashenka/Renderer/Shader.h"

// TODO: Remove!
typedef unsigned int GLenum;

namespace Mashenka
{
    class OpenGLShader : public Shader
    {
    public:
        // OpenGL specific solution for the Shader Class
        // Constructor
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);

        // Destructor
        ~OpenGLShader() override;

        // Bind and Unbind the shader
        void Bind() const override;
        void Unbind() const override;

        // Upload uniform functions for different types
        void UploadUniformInt(const std::string& name, int value) const;
        void UploadUniformFloat(const std::string& name, float value) const;
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;
        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
    private:
        std::string Readfile (const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        // Shader program id
        uint32_t m_RendererID;
    };
}

