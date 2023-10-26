#include "mkpch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace Mashenka
{
    void Renderer::BeginScene()
    {
        // Prepare for the next frame
        
    }

    void Renderer::EndScene()
    {
        // Render the next frame
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray> vertexArray)
    {
        // Submit the vertex array to the RendererCommand
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
