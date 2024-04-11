#include "GameLayer.h"

using namespace Mashenka;

GameLayer::GameLayer() : Layer("GameLayer"), m_CameraController(1280.0f / 720.0f)
{

}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
    Layer::OnAttach();

    // setup the basic fonts for the game
    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);

    // load basic assets
    m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
    m_Snake = Texture2D::Create("assets/textures/Snake.png");
}

void GameLayer::OnDetach()
{
    Layer::OnDetach();
}

void GameLayer::OnUpdate(TimeStep ts)
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
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();
    }


    // Begin scene
    {
        MK_PROFILE_SCOPE("Render Draw");
        Renderer2D::BeginScene(m_CameraController.GetCamera());
        Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, 0.0f, m_Snake, 1.0f);
        Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, 0.0f, m_CheckerboardTexture, 10.f);
        Renderer::EndScene();
    }
}

void GameLayer::OnImGuiRender()
{
    Layer::OnImGuiRender();
}

void GameLayer::OnEvent(Event& event)
{
    Layer::OnEvent(event);
}

