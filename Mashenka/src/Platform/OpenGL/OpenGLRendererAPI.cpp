#include "mkpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Mashenka
{
    // This is the OpenGL message callback function, it is used to print out the OpenGL error messages
    // param: source, type, id, severity, length, message, userParam
    // source: the source of the message, such as API, window system, shader compiler, etc.
    // type: the type of the message, such as error, performance, deprecated behavior, etc.
    // id: the id of the message, such as a message code
    // severity: the severity of the message, such as high, medium, low, etc.
    // length: the length of the message
    // message: the message
    // userParam: the user parameter
    void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH: MK_CORE_ERROR(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM: MK_CORE_WARN(message);
            return;
        case GL_DEBUG_SEVERITY_LOW: MK_CORE_INFO(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: MK_CORE_TRACE(message);
            return;
        default: break;
        }
        MK_CORE_ASSERT(false, "Unknown severity level!");
    }

    void OpenGLRendererAPI::Init()
    {
        MK_PROFILE_FUNCTION(); // Profiling

        // debug message
#ifdef MK_DEBUG
        glEnable(GL_DEBUG_OUTPUT); // enable the debug output
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // enable the synchronous debug output
        glDebugMessageCallback(OpenGLMessageCallback, nullptr); // set the callback function

        // filter out the messages we don't care about
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        // Opengl function
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        // Opengl function
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        // Opengl function
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        // Opengl function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<Mashenka::VertexArray>& vertexArray)
    {
        // Opengl function
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture, so that we can use the texture slot for other textures
    }
}
