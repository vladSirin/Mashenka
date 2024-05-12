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
    glm::vec2 m_PlayerPos;

    enum class GameState
    {
        Play = 0,
        MainMenu = 1,
        GameOver = 2
    };

    GameState m_State = GameState::MainMenu;

    Background m_Background; //TODO: move this to level
};
