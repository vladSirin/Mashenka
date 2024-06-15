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
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

        // Destructor
        ~OpenGLShader() override;

        // Bind and Unbind the shader
        void Bind() const override;
        void Unbind() const override;

        // Set the uniform
        void SetInt(const std::string& name, int value) override;
        void SetIntArray(const std::string& name, int* values, uint32_t count) override;
        void SetFloat(const std::string& name, float value) override;
        void SetFloat2(const std::string& name, const glm::vec2& value) override;
        void SetFloat3(const std::string& name, const glm::vec3& value) override;
        void SetFloat4(const std::string& name, const glm::vec4& value) override;
        void SetMat4(const std::string& name, const glm::mat4& value) override;

        virtual const std::string& GetName() const override {return m_Name; }

        // Upload uniform functions for different types
        void UploadUniformInt(const std::string& name, int value) const;
        void UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const;
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
        std::string m_Name;
    };
}

