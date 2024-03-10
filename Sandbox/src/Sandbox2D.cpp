#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) // 1280.0f / 720.0f is the aspect ratio
{
}

void Sandbox2D::OnAttach()
{
    MK_PROFILE_FUNCTION(); // Profiling
    m_CheckerboardTexture = Mashenka::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    MK_PROFILE_FUNCTION(); // Profiling
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Mashenka::TimeStep ts)
{
    MK_PROFILE_FUNCTION();
    Layer::OnUpdate(ts);

    // Update
    {
        MK_PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(ts);
    }

    //render
    {
        MK_PROFILE_SCOPE("Render Prep");
        Mashenka::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Mashenka::RenderCommand::Clear();
    }


    // Begin scene
    {
        MK_PROFILE_SCOPE("Render Draw");
        Mashenka::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Mashenka::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
        Mashenka::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, 0.0f, { 0.2f, 0.3f, 0.8f, 1.0f });
        Mashenka::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, 0.0f, m_CheckerboardTexture, 10.f);
        Mashenka::Renderer::EndScene();
    }

}

void Sandbox2D::OnImGuiRender()
{
    MK_PROFILE_FUNCTION();
    Layer::OnImGuiRender();

    // Setup a settings window and use Square Color as a color picker
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Mashenka::Event& e)
{
    MK_PROFILE_SCOPE("Sandbox2D::OnEvent");
    Layer::OnEvent(e);
    m_CameraController.OnEvent(e); // The camera controller is used to handle events
}
