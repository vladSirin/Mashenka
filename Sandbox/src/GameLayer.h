#pragma once
#include "Background.h"
#include "Level.h"
#include "Mashenka.h"
#include "Player.h"
#include "imgui/imgui.h"

using namespace Mashenka;

class GameLayer : public Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(TimeStep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;

    bool OnEnterKeyPressed(KeyPressedEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

private:
    OrthographicCameraController m_CameraController;
    Ref<Texture2D> m_CheckerboardTexture;
    Ref<Texture2D> m_Snake;
    ImFont* m_Font;
    float m_Time = 0.0f;
    bool m_Blink = false;
    Level m_Level;
    glm::vec2 m_PlayerPos = {0.0f, 0.0f};

    enum class GameState
    {
        Play = 0,
        MainMenu = 1,
        GameOver = 2
    };

    GameState m_State = GameState::MainMenu;
    int m_FPS = 0;

    void ImGuiRenderRewardIndicator();
    glm::vec2 CalculateDirection(const glm::vec2& from, const glm::vec2& to);
    ImVec2 CalculateArrowPosition(const ImVec2& windowSize, const ImVec2& center, const glm::vec2& direction);
    void DrawArrow(ImDrawList* draw_list, const ImVec2& start, float angle, float arrow_size, ImU32 color);
    void ImGuiRenderArrow(const glm::vec2& playerPosition, const glm::vec2& rewardPosition);


    glm::vec4 CAMERA_PROJECTION = {-1280.0f / 720.0f, 1280.0f / 720.0f, 1.0f, -1.0f};
    //glm::vec4 CAMERA_PROJECTION = {-16.0f, 16.0f, 9.0f, -9.0f};
};
