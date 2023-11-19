#include "mkpch.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"



namespace Mashenka
{
        /*
    * Shader Compilation: Individual vertex and fragment shaders are compiled.
    Shader Linking: Compiled shaders are linked into a program.
    Use Program: The program is set as the active program for the OpenGL context.
    Draw Call: OpenGL uses the active program when performing draw calls.
     */

        static GLenum ShaderTypeFromString(const std::string& type)
        {
            // Vertex
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            // Fragment
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            // Assertion
            MK_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }

        OpenGLShader::OpenGLShader(const std::string& filepath)
        {
            // Read the file
            std::string source = Readfile(filepath);
            // Preprocess the file
            auto shaderSources = PreProcess(source);
            // Compile the shader
            Compile(shaderSources);

            // Extract name from filepath
            auto lastSlash = filepath.find_last_of("/\\");
            lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
            auto lastDot = filepath.rfind('.');
            auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
            m_Name = filepath.substr(lastSlash, count);
        }

        std::string OpenGLShader::Readfile(const std::string& filepath)
        {
            // read the file
            std::string result;
            std::ifstream in(filepath, std::ios::in | std::ios::binary);
            if (in)
            {
                // read the file
                in.seekg(0, std::ios::end);
                result.resize(in.tellg());
                in.seekg(0, std::ios::beg);
                in.read(&result[0], result.size());
                in.close();
            }
            else
            {
                MK_CORE_ERROR("Could not open file '{0}'", filepath);
            }
            // return the result
            return result;
        }

        std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
        {
            // create a map of the shader sources
            std::unordered_map<GLenum, std::string> shaderSources;
            const char* typeToken = "#type";
            size_t typeTokenLength = strlen(typeToken);
            size_t pos = source.find(typeToken, 0); // Start of shader type declaration line
            while (pos != std::string::npos)
            {
                size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
                MK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
                size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
                std::string type = source.substr(begin, eol - begin);
                MK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

                size_t nextLinePos = source.find_first_not_of("\r\n", eol); // Start of shader code after shader type declaration line
                MK_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
                pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line

                shaderSources[ShaderTypeFromString(type)] =
                    (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
            }
            // return the shader sources
            return shaderSources;
        }

        void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
        {
            // Get a program object.
            GLuint program = glCreateProgram();
            MK_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
            std::array<GLenum, 2> glShaderIDs;
            int glShaderIDIndex = 0;
            for (auto& kv : shaderSources)
            {
                GLenum type = kv.first;
                const std::string& source = kv.second;

                // Create an empty shader object of the specified type.
                GLuint shader = glCreateShader(type);

                // Replace the source code in a shader object.
                const GLchar* sourceCStr = source.c_str();
                glShaderSource(shader, 1, &sourceCStr, 0);

                // Compile the shader object.
                glCompileShader(shader);

                // Check the shader object compile status
                GLint isCompiled = 0;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE)
                {
                    GLint maxLength = 0;
                    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                    // The maxLength includes the NULL character
                    std::vector<GLchar> infoLog(maxLength);
                    glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                    // We don't need the shader anymore.
                    glDeleteShader(shader);

                    // Use the infoLog as you see fit.
                    MK_CORE_ERROR("{0}", infoLog.data());
                    MK_CORE_ASSERT(false, "Shader compilation failure!");
                    break;
                }

                // Attach a shader object to a program object.
                glAttachShader(program, shader);
                glShaderIDs[glShaderIDIndex++] = shader;
            }

            m_RendererID = program;

            // link the program object
            glLinkProgram(program);

            // Note the different functions here: glGetProgram* instead of glGetShader*.
            GLint isLinked = 0;
            glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
            if (isLinked == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

                // We don't need the program anymore.
                glDeleteProgram(program);
                // Don't leak shaders either.
                for (auto id : glShaderIDs)
                    glDeleteShader(id);

                // Use the infoLog as you see fit.
                MK_CORE_ERROR("{0}", infoLog.data());
                MK_CORE_ASSERT(false, "Shader link failure!");
                return;
            }
        }

    // Constructor
    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_Name(name)
    {
            std::unordered_map<GLenum,std::string> shaderSources;
            shaderSources[GL_VERTEX_SHADER] = vertexSrc;
            shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
            Compile(shaderSources);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(
            m_RendererID); // Install the program object specified by program as part of current rendering state.
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0); // Uninstall the current program object.
    }

    /*
     * ==============================SET UNIFORMS==============================
     */
    void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }

        // set the uniform matrix value
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    // Set uniforms for screen space transformation
    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }

        // set the uniform matrix value
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    // upload uniform for vec3
    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }

        // set the uniform matrix value
        glUniform3f(location, vector.x, vector.y, vector.z);
    }
}
