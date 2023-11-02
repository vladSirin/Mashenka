#include "mkpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include "glm/gtc/type_ptr.inl"

namespace Mashenka
{
    /*
    * Shader Compilation: Individual vertex and fragment shaders are compiled.
    Shader Linking: Compiled shaders are linked into a program.
    Use Program: The program is set as the active program for the OpenGL context.
    Draw Call: OpenGL uses the active program when performing draw calls.
     */

    // Constructor
    Mashenka::Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        // Create an empty vertex shader handle
        // A shader object is used to maintain the source code strings that define a shader.
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string 's .c_str is NULL character terminated.
        // The length of the string is implicitly determined by the first null character.
        const GLchar* source = (const GLchar*)vertexSrc.c_str();

        // Replace the source code in a shader object, in this case replace vertex shader source code with vertexSrc
        // The last parameter of glShaderSource is an array of string lengths.
        // If this parameter is NULL, each string is assumed to be null terminated.
        // Otherwise, it is an array of integers where each element is the length of the corresponding string
        // (the null character is not counted as part of the string length).
        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        // Check for errors, query the compile status, if failed, print the error message
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the Null character
            std::vector<GLchar> infoLog(maxLength);
            // Retrieve the info log
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore
            glDeleteShader(vertexShader);

            MK_CORE_ERROR("{0}", infoLog.data());
            MK_CORE_ASSERT(false, "Vertex shader compilation failure!");
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = (const GLchar*)fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        // Check if the compilation is succeeded
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the Null Character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore
            glDeleteShader(fragmentShader);
            // Either of them , don't leak shaders.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit
            MK_CORE_ERROR("{0}", infoLog.data());
            MK_CORE_ASSERT(false, "Fragment shader compilation failure!");
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object. Which is the container for the shaders.
        m_RendererID = glCreateProgram();
        GLuint program = m_RendererID;

        // Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the Null character
            std::vector<GLchar> infolog(maxLength);
            glGetShaderInfoLog(program, maxLength, &maxLength, &infolog[0]);

            // We don't need the program anymore
            glDeleteProgram(program);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.
            MK_CORE_ERROR("{0}", infolog.data());
            MK_CORE_ASSERT(false, "Shader link failure!");
            return;
        }

        // Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
    }

    Mashenka::Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    void Mashenka::Shader::Bind() const
    {
        glUseProgram(
            m_RendererID); // Install the program object specified by program as part of current rendering state.
    }

    void Mashenka::Shader::Unbind() const
    {
        glUseProgram(0); // Uninstall the current program object.
    }

    // Set uniforms for screen space transformation
    void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
        {
            MK_CORE_ERROR("Uniform {0} not found!", name);
        }

        // set the uniform matrix value
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}
