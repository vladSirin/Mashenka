#include "Player.h"

#include "imgui/imgui.h"

using namespace Mashenka;

Player::Player()
{
}

void Player::LoadAssets()
{
    m_SnakeTexture = Texture2D::Create("assets/textures/Snake.png");
}

void Player::OnUpdate(Mashenka::TimeStep ts)
{
    // based on the key that is pressed, I should swap the direction and reset the velocity of the snake
    if (Input::IsKeyPressed(MK_KEY_UP))
        m_Direction = Direction::UP;
    else if (Input::IsKeyPressed(MK_KEY_DOWN))
        m_Direction = Direction::DOWN;
    else if (Input::IsKeyPressed(MK_KEY_LEFT))
        m_Direction = Direction::LEFT;
    else if (Input::IsKeyPressed(MK_KEY_RIGHT))
        m_Direction = Direction::RIGHT;

    UpdateVelocity();
    m_Position += m_Velocity * (float)ts;
}

void Player::OnRender()
{
    Renderer2D::DrawRotatedQuad({m_Position.x, m_Position.y, 0.5}, {1.0f, 1.3f}, glm::radians(GetRotation()), m_SnakeTexture);
}

void Player::OnImGuiRender()
{
    ImGui::DragFloat("Speed", &m_DirectionalSpeed, 0.1f);
}

void Player::Reset()
{
    m_Position = {-10.0f, 0.0f};
    m_DirectionalSpeed = DEFAULT_DIRECTIONAL_SPEED;
    m_Direction = Direction::RIGHT;
    UpdateVelocity();
}

// private functions
void Player::UpdateVelocity()
{
    switch (m_Direction)
    {
    case Direction::UP:
        m_Velocity = {0.0f, m_DirectionalSpeed * 1.0f};
        break;
    case Direction::DOWN:
        m_Velocity = {0.0f, m_DirectionalSpeed * -1.0f};
        break;
    case Direction::LEFT:
        m_Velocity = {m_DirectionalSpeed * -1.0f, 0.0f};
        break;
    case Direction::RIGHT:
        m_Velocity = {m_DirectionalSpeed * 1.0f, 0.0f};
        break;
    }
}
