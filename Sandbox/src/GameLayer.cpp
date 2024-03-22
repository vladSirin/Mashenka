#include "GameLayer.h"

#include "Random.h"

using namespace Mashenka;

GameLayer::GameLayer() : Layer("GameLayer"), m_Snake(20,20), m_Food(20, 20), m_Level(1280, 820)
{
    auto& window = Application::Get().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());

    Random::Init();
}

void GameLayer::OnAttach()
{
    Layer::OnAttach();
    m_Level.Init();

    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/Menlo-Regular.ttf", 120.0f);
}

void GameLayer::OnDetach()
{
    Layer::OnDetach();
}

void GameLayer::OnImGuiRender()
{
    Layer::OnImGuiRender();

    // UIs
    switch (m_State)
    {
    // When Playing, showing the player score and current level
    case GameState::Play:
        {
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);

            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
            break;
        }

    // When in MainMenu, showing the option to start the game or quit, and highest score maybe?
    case GameState::MainMenu:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width*0.5f - 300.0f;
            pos.y += 50.0f;
            if(m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
            break;
        }

    // When GameOver, showing current score, highest score and option to click and try again
    case GameState::GameOver:
        {
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width *0.5f -300.0f;
            pos.y +=50.0f;
            if(m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

            pos.x += 200.0f;
            pos.y += 150.0f;
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 40.0f, pos, 0xffffffff, scoreStr.c_str());
            break;
        }
    }
}

void GameLayer::OnUpdate(Mashenka::TimeStep ts)
{
    Layer::OnUpdate(ts);
    m_Time += ts;
    if ((int)(m_Time * 10.0f) % 8 > 4)
        m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
        m_State = GameState::GameOver;

    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    m_Camera->SetPosition({playerPos.x, playerPos.y, 0.0f});

    switch (m_State)
    {
    case GameState::Play:
        {
            m_Level.OnUpdate(ts);
            break;;
        }
    }

    // Render
    Mashenka::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1});
    Mashenka::RenderCommand::Clear();

    Mashenka::Renderer2D::BeginScene(*m_Camera);
    m_Level.OnRender();
    Mashenka::Renderer2D::EndScene();
}

void GameLayer::OnEvent(Mashenka::Event& event)
{
    Layer::OnEvent(event);
}

bool GameLayer::OnMouseButtonPressed(Mashenka::MouseButtonPressedEvent& e)
{
    return false;
}

bool GameLayer::OnWindowResized(Mashenka::WindowResizeEvent& e)
{
    return false;
}

void GameLayer::UpdateSnake(float ts)
{
}

void GameLayer::CheckCollision()
{
}

void GameLayer::SpawnFood()
{
}

void GameLayer::ResetGame()
{
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
}
