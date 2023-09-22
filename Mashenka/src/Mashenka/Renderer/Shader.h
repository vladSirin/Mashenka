#pragma once
#include <string>


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

    private:
        // Shader ID
        uint32_t m_RendererID;
    
    };
}

