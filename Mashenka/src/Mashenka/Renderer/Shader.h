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

        // Set the uniform
        virtual void SetInt(const std::string& name, int value) = 0;
        virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
        virtual void SetFloat(const std::string& name, float value) = 0;
        virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

        virtual const std::string& GetName() const = 0; // get the name of the shader
        
        // create a shader
        // the type is the type of the shader
        // the source is the source code of the shader
        // return the id of the shader
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

        // Create a shader from file path
        static Ref<Shader> Create(const std::string& filepath);


    };

    // Shader Library Class
    // Why: to store all the shaders in the application, so that we can access them later
    // How: using a map to store the shaders, the key is the name of the shader, the value is the shader itself
    //     using a static function to get the shader from the library
    // When a class without constructor is created, the default constructor is called, along with the default destructors of the member variables
    class ShaderLibrary
    {
    public:
        void add(const std::string& name, const std::shared_ptr<Shader>& shader);
        void add(const std::shared_ptr<Shader>& shader);

        // load the shader from the file path. The name of the shader is the name of the file
        Ref<Shader> Load(const std::string& filepath);

        // load the shader from the file path. the name reference is the name of the shader
        Ref<Shader> Load(const std::string& name, const std::string& filepath);
            
        Ref<Shader> Get(const std::string& name);
        bool Exists(const std::string& name) const;
            
    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
}

