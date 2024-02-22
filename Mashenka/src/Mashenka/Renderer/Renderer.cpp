#include "mkpch.h"
#include "Mashenka/Renderer/Renderer.h"
#include "Mashenka/Renderer/Renderer2D.h"


namespace Mashenka
{
    // Initialize the scene data
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<SceneData>();

    // Init
    void Renderer::Init()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // Initialize the renderer API
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        // Set the viewport
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        // Set the view projection matrix of the scene
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
        // Render the next frame
    }

    // Submit the vertex array to the RendererCommand
    // This is the function that will be called by the application
    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        MK_PROFILE_FUNCTION(); // Profiling
        shader->Bind(); // Bind the shader

        // Set the uniform matrix in the shader
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);
        
        // Submit the vertex array to the RendererCommand
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
