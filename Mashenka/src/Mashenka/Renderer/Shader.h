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
        virtual ~Shader() = default;

        // Bind and Unbind the shader
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // create a shader
        // the type is the type of the shader
        // the source is the source code of the shader
        // return the id of the shader
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

        // Create a shader from file path
        static Shader* Create(const std::string& filepath);
        
    };
}

