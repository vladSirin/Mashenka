#pragma once
#include <string>
#include "glm/glm.hpp"


namespace Mashenka
{
    // OpenGl Shader Class
    class Shader
    {
    public:
        // Constructor
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        // Bind and Unbind the shader
        void Bind() const;
        void Unbind() const;

        // Set uniforms for screen space transformation
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        // Shader ID
        uint32_t m_RendererID;
    
    };
}

