#include "GameLayer.h"

using namespace Mashenka;

GameLayer::GameLayer() : Layer("GameLayer"), m_CameraController(1280.0f / 720.0f)
{
}

void GameLayer::OnAttach()
{
    Layer::OnAttach();

    // setup the basic fonts for the game
    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);

    //TODO: Move the background scale and setups to level, update the background based player crossing tiles
    m_Background = Background(1, 1, 1280, 720);
    m_Background.LoadAssets();
    

    // Init Level
    m_Level.Init();
}

void GameLayer::OnDetach()
{
    Layer::OnDetach();
}

void GameLayer::OnUpdate(TimeStep ts)
{
    MK_PROFILE_FUNCTION();
    Layer::OnUpdate(ts);

    m_Time += ts;
    if ((int)(m_Time * 10.0f) % 8 > 4)
        m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
        m_State = GameState::GameOver;
    
    if (m_State == GameState::Play)
    {
        m_Level.OnUpdate(ts);
    }

    // Calculate Camera positions based on player
    m_PlayerPos = m_Level.GetPlayer().GetPosition();
    m_CameraController.SetPosition({ m_PlayerPos.x, m_PlayerPos.y, 0.0f });

    // Update
    {
        MK_PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(ts);
    }

    //render
    {
        MK_PROFILE_SCOPE("Render Prep");
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();
    }


    // Begin scene
    {
        MK_PROFILE_SCOPE("Render Draw");
        Renderer2D::BeginScene(m_CameraController.GetCamera());

        // Render Level
        m_Background.OnRender(m_PlayerPos);
        m_Level.OnRender();

        Renderer::EndScene();
    }
}

void GameLayer::OnImGuiRender()
{
    Layer::OnImGuiRender();

    switch (m_State)
    {
    case GameState::Play:
        {
            //TODO: display the player score when playing the game
            uint32_t playerScore = 10;
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 60.0f, ImGui::GetWindowPos(), 0xFFFF0000
                                                    , scoreStr.c_str());
            break;
        }
    case GameState::MainMenu:
        {
            // display the main menu when in menu state in middle
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xFFFF0000
                                                        , "Press ENTER to Play!");
            break;
        }
    case GameState::GameOver:
        {
            // display "click to play" info and the score when the game is over
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xFFFF0000
                                                        , "Press ENTER to Play!");

            pos.x += 200.0f;
            pos.y += 150.0f;
            //TODO: display the player score when the game is over
            uint32_t playerScore = 10;
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 20.0f, ImGui::GetWindowPos(), 0xFFFF0000
                                                    , scoreStr.c_str());
            break;
        }
    }
}

void GameLayer::OnEvent(Event& event)
{
    Layer::OnEvent(event);
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>(MK_BIND_EVENT_FN(GameLayer::OnWindowResized));
    dispatcher.Dispatch<KeyPressedEvent>(MK_BIND_EVENT_FN(GameLayer::OnEnterKeyPressed));
}

bool GameLayer::OnEnterKeyPressed(KeyPressedEvent& e)
{
    if (m_State == GameState::GameOver)
        m_Level.Reset();

    m_State = GameState::Play;
    return false;
}

bool GameLayer::OnWindowResized(WindowResizeEvent& e)
{
    m_CameraController.OnEvent(e);
    m_Background.OnEvent(e); //TODO: move this to level
    return false;
}
