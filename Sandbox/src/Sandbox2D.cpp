#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"


Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) // 1280.0f / 720.0f is the aspect ratio
{
}

void Sandbox2D::OnAttach()
{
    m_CheckerboardTexture = Mashenka::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Mashenka::TimeStep ts)
{
    Layer::OnUpdate(ts);

    // Update
    m_CameraController.OnUpdate(ts);

    //render
    Mashenka::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Mashenka::RenderCommand::Clear();

    // Begin scene
    Mashenka::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Mashenka::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    Mashenka::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
    Mashenka::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
    
    Mashenka::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    Layer::OnImGuiRender();

    // Setup a settings window and use Square Color as a color picker
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Mashenka::Event& e)
{
    Layer::OnEvent(e);
    m_CameraController.OnEvent(e); // The camera controller is used to handle events
}
