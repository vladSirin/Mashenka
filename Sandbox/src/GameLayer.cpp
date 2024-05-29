#include "GameLayer.h"

using namespace Mashenka;

GameLayer::GameLayer() : Layer("GameLayer"), m_CameraController(1280.0f / 720.0f)
{
    /** Due to the primitive framework of the engine this version, the actual projection is {-aspectRatio, aspectRatio, -1.0f, 1.0f}
     ** Which leads to the smaller scale of the assets, which is not ideal
     ** A More proper way would be setting up the projection in the constructor, also provide the getter and setter for the projection
     */

    //TODO: remove this when test is over.
    m_CameraController.GetCamera().SetProjection(CAMERA_PROJECTION[0], CAMERA_PROJECTION[1], CAMERA_PROJECTION[2],
                                                 CAMERA_PROJECTION[3]);
}

void GameLayer::OnAttach()
{
    Layer::OnAttach();

    // setup the basic fonts for the game
    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 120.0f);

    // Init Level
    m_Level.Init();
    m_Level.SetCameraProjection(CAMERA_PROJECTION);
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
    m_Level.SetGameTime(m_Time); // set time in level

    if ((int)(m_Time * 10.0f) % 8 == 4)
    {
        m_Blink = !m_Blink;
        m_FPS = static_cast<int>(1.0f / ts);
    }

    if (m_Level.IsGameOver())
        m_State = GameState::GameOver;

    if (m_State == GameState::Play)
    {
        m_Level.OnUpdate(ts);
    }

    // Calculate Camera positions based on player
    m_PlayerPos = m_Level.GetPlayer().GetPosition();
    m_CameraController.SetPosition({m_PlayerPos.x, m_PlayerPos.y, 0.0f});

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
        m_Level.OnRender();

        Renderer::EndScene();
    }
}

void GameLayer::OnImGuiRender()
{
    Layer::OnImGuiRender();
    
    ImGuiRenderRewardIndicator(); //TODO： fix bug that sometimes does not show indicator

    {
        auto pos = ImGui::GetWindowPos();
        pos.x += 50.0f;
        pos.y += 50.0f;
        std::string scoreStr = std::string("FPS: ") + std::to_string(m_FPS);
        ImGui::GetForegroundDrawList()->AddText(m_Font, 20.0f, pos, 0xFF808080
                                                , scoreStr.c_str());
    }

    switch (m_State)
    {
    case GameState::Play:
        {
            uint32_t playerScore = m_Level.GetPlayerScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);

            auto pos = ImGui::GetWindowPos();
            pos.x += 100.0f;
            pos.y += 100.0f;
            ImGui::GetForegroundDrawList()->AddText(m_Font, 60.0f, pos, 0xFFFF0000
                                                    , scoreStr.c_str());
            break;
        }
    case GameState::MainMenu:
        {
            // display the main menu when in menu state in middle
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5 - 480.0f;
            pos.y += height * 0.5;
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
            pos.x += width * 0.5 - 560.0f;
            pos.y += height * 0.2;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 80.0f, pos, 0xFF0000FF
                                                        , "Game Over, Press Enter to Play Again!");

            //TODO: display the player score when the game is over
            uint32_t playerScore = m_Level.GetPlayerScore();
            std::string scoreStr = std::string("Final Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 80.0f, {pos.x, pos.y + 80.0f}, 0xFF0000FF
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
    m_CameraController.GetCamera().SetProjection(-16.0f, 16.0f, 9.0f, -9.0f);
    return false;
}

void GameLayer::ImGuiRenderRewardIndicator()
{
    //Drawing an arrow if the reward is out of the player view
    // Firstly we need to iterate through the rewards, then for each of them, we check:
    // * if the reward is out of the player view
    // * if so, we draw an arrow from the player position pointing to the reward
    // * the arrow should be located on the edge of the player view, using a red color
    for (auto& reward : m_Level.GetRewards())
    {
        if (m_Level.IsPositionOutofView(reward.GetPosition(), m_Level.GetPlayer().GetPosition(), {
                                            CAMERA_PROJECTION[0], CAMERA_PROJECTION[1], CAMERA_PROJECTION[2],
                                            CAMERA_PROJECTION[3]
                                        }))
        {
            // MK_CORE_INFO("Reward Position: {0}, {1}, Out of View", reward.GetPosition().x, reward.GetPosition().y);
            //DRAW ARROW
            ImGuiRenderArrow(m_Level.GetPlayer().GetPosition(), reward.GetPosition());
        }
    }
}


void GameLayer::DrawArrow(ImDrawList* draw_list, const ImVec2& start, float angle, float arrow_size, ImU32 color)
{
    ImVec2 direction = ImVec2(cos(angle), sin(angle));
    ImVec2 perp = ImVec2(-direction.y, direction.x);

    ImVec2 arrow_tip = start;
    ImVec2 arrow_base1 = ImVec2(start.x - arrow_size * direction.x + arrow_size * 1.0f * perp.x,
                                start.y - arrow_size * direction.y + arrow_size * 1.0f * perp.y);
    ImVec2 arrow_base2 = ImVec2(start.x - arrow_size * direction.x - arrow_size * 1.0f * perp.x,
                                start.y - arrow_size * direction.y - arrow_size * 1.0f * perp.y);

    draw_list->AddTriangleFilled(arrow_tip, arrow_base1, arrow_base2, color);
}

void GameLayer::ImGuiRenderArrow(const glm::vec2& playerPosition, const glm::vec2& rewardPosition)
{
    // Get the draw list
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    // Get window size and center
    ImVec2 windowSize = {1280, 720};
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowCenter = ImVec2(windowPos.x + windowSize.x / 2, windowPos.y + windowSize.y / 2);

    // Calculate the direction from the player to the reward
    glm::vec2 direction = CalculateDirection(playerPosition, rewardPosition);

    // Calculate the position on the edge of the window
    ImVec2 arrowPos = CalculateArrowPosition(windowSize, windowCenter, direction);

    // Calculate the angle of the arrow
    float angle = atan2(direction.y, direction.x);

    // Draw the arrow using the color red
    ImU32 colorRed = IM_COL32(255, 0, 0, 255); // Fully opaque red
    DrawArrow(draw_list, arrowPos, angle, 20.0f, colorRed);
}

glm::vec2 GameLayer::CalculateDirection(const glm::vec2& from, const glm::vec2& to)
{
    glm::vec2 direction = to - from;
    return glm::normalize(direction);
}

ImVec2 GameLayer::CalculateArrowPosition(const ImVec2& windowSize, const ImVec2& center, const glm::vec2& direction)
{
    
    ImVec2 edgePosition = center;

    // Determine which edge of the window to place the arrow
    if (fabs(direction.x) > fabs(direction.y))
    {
        // Place on left or right edge
        if (direction.x > 0)
        {
            edgePosition.x += (windowSize.x / 2 - 20.0f); // Right edge, this is a magic fix, not sure why
        }
        else
        {
            edgePosition.x -= windowSize.x / 2 - 20.0f; // Left edge
        }
        edgePosition.y = center.y + direction.y * (windowSize.x / 2.0f - 20.0f) / fabs(direction.x);
    }
    else
    {
        // Place on top or bottom edge
        if (direction.y > 0)
        {
            edgePosition.y += windowSize.y / 2 - 20.0f; // Bottom edge, this is a magic fix, not sure why
        }
        else
        {
            edgePosition.y -= windowSize.y / 2 - 20.0f; // Top edge
        }
        edgePosition.x = center.x + direction.x * (windowSize.y / 2.0f - 20.0f) / fabs(direction.y);
    }

    // Ensure the arrow stays within the window bounds
    edgePosition.x = glm::clamp(edgePosition.x, center.x - windowSize.x / 2.0f + 20.0f, center.x + windowSize.x / 2.0f - 20.0f);
    edgePosition.y = glm::clamp(edgePosition.y, center.y - windowSize.y / 2.0f + 20.0f, center.y + windowSize.y / 2.0f - 20.0f);

    return edgePosition;
}
