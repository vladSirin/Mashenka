#pragma once
#include "Food.h"
#include "Level.h"
#include "Mashenka.h"
#include "Snake.h"
#include <imgui/imgui.h>

class GameLayer : public Mashenka::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void OnUpdate(Mashenka::TimeStep ts) override;
    void OnEvent(Mashenka::Event& event) override;

    // input
    bool OnMouseButtonPressed(Mashenka::MouseButtonPressedEvent& e);
    bool OnWindowResized(Mashenka::WindowResizeEvent& e);

    // additional method for game logic
    void UpdateSnake(float ts);
    void CheckCollision();
    void SpawnFood();
    void ResetGame();
private:
    void CreateCamera(uint32_t width, uint32_t height);
private:
    // Game State variables, e.g. snake, food, score
    Mashenka::Scope<Mashenka::OrthographicCamera> m_Camera;
    
    Snake m_Snake;
    Food m_Food;
    int m_Score;
    bool m_GameOver;
    Level m_Level;
    ImFont* m_Font;
    float m_Time = 0.0f;
    bool m_Blink = false;

    enum class GameState
    {
        Play = 0, MainMenu = 1, GameOver = 2
    };

    GameState m_State = GameState::MainMenu;
    
};
