#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f), m_SquareColor({0.2, 0.3, 0.8, 1.0f}) // 1280.0f / 720.0f is the aspect ratio
{
    m_CameraController.GetCamera().SetPosition({0.0f, 0.0f, 0.0f});
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
    Mashenka::Renderer2D::ResetStats();
    {
        MK_PROFILE_SCOPE("Render Prep");
        Mashenka::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mashenka::RenderCommand::Clear();
    }


    // Begin scene
    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;


        MK_PROFILE_SCOPE("Render Draw");
        Mashenka::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Mashenka::Renderer2D::DrawRotatedQuad({-0.8f, 0.7f}, {1.8f, 1.8f}, rotation, {0.8f, 0.2f, 0.3f, 1.0f});
        Mashenka::Renderer2D::DrawRotatedQuad({0.9f, -0.4f}, {0.3f, 0.55f}, -rotation / 2, m_SquareColor);
        //Mashenka::Renderer2D::DrawQuad({-1.0f, 0.0f}, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        //Mashenka::Renderer2D::DrawQuad({0.5f, -0.5f}, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Mashenka::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
        Mashenka::Renderer2D::DrawQuad({0.0f, 0.0f, 0.0f}, {5.0f, 5.0f}, m_CheckerboardTexture, 20.0f);
        Mashenka::Renderer2D::EndScene();

        // Trying to draw a big num of quads for the drawcall
        Mashenka::Renderer2D::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x <5.0f;  x += 0.5f)
            {
                glm::vec4 color = {(x+5.0f) / 10.0f, 0.4f, (y+5.0f)/ 10.0f, 0.7f};
                Mashenka::Renderer2D::DrawQuad({x,y}, {0.45f, 0.45f}, color);
            }
        }
        Mashenka::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    MK_PROFILE_FUNCTION();
    Layer::OnImGuiRender();

    // Setup a settings window and use Square Color as a color picker
    ImGui::Begin("Settings");

    // Displaying Rendering Stats for the current scene
    auto stats = Mashenka::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Mashenka::Event& e)
{
    MK_PROFILE_SCOPE("Sandbox2D::OnEvent");
    Layer::OnEvent(e);
    m_CameraController.OnEvent(e); // The camera controller is used to handle events
}
