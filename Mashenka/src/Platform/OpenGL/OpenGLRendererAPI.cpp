#include "mkpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Mashenka
{
    void OpenGLRendererAPI::Init()
    {
        MK_PROFILE_FUNCTION(); // Profiling
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
