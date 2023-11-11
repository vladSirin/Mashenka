#include "mkpch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Mashenka
{
    void OpenGLRendererAPI::Init()
    {
        // Opengl function
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_DEPTH_TEST);
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

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<Mashenka::VertexArray>& vertexArray)
    {
        // Opengl function
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }
}
