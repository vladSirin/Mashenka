#pragma once

#include <Mashenka.h>

enum Direction : int
{
    UP = 0,
    DOWN = 180,
    LEFT = 90,
    RIGHT = 270,
};

class Player
{
public:
    Player();

    void LoadAssets();

    void OnUpdate(Mashenka::TimeStep ts);
    void OnRender();

    void OnImGuiRender();

    Direction GetDirection() const { return m_Direction; }
    float GetRotation() { return (float)GetDirection(); }

    void Reset();

private:
    inline static constexpr float DEFAULT_DIRECTIONAL_SPEED = 1.0f;


    float m_DirectionalSpeed = DEFAULT_DIRECTIONAL_SPEED; //directional speed
    glm::vec2 m_Position = {-10.0f, 0.0f};
    glm::vec2 m_Velocity = {5.0f, 0.0f}; //this is updated based on the direction and m_Speed
    Direction m_Direction = Direction::RIGHT;

    Mashenka::Ref<Mashenka::Texture2D> m_SnakeTexture;

    void UpdateVelocity();
};
