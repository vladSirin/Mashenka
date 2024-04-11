#pragma once
#include "Mashenka.h"
#include "imgui/imgui.h"

using namespace Mashenka;

class GameLayer : public Layer
{
public:
    GameLayer();
    virtual ~GameLayer();

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(TimeStep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;

private:
    OrthographicCameraController m_CameraController;
    Ref<Texture2D> m_CheckerboardTexture;
    Ref<Texture2D> m_Snake;
    ImFont* m_Font;
    float m_Time = 0.0f;

    enum class GameState
    {
        Play = 0,
        MainMenu = 1,
        GameOver = 2
    };

    GameState m_State = GameState::MainMenu;
};
