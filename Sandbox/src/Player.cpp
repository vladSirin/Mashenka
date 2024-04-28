#include "Player.h"

#include "glm/ext/matrix_transform.hpp"
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
    if (Input::IsKeyPressed(MK_KEY_UP) && GetDirection() != DOWN)
        m_Direction = Direction::UP;
    else if (Input::IsKeyPressed(MK_KEY_DOWN) && GetDirection() != UP)
        m_Direction = Direction::DOWN;
    else if (Input::IsKeyPressed(MK_KEY_LEFT) && GetDirection() != RIGHT)
        m_Direction = Direction::LEFT;
    else if (Input::IsKeyPressed(MK_KEY_RIGHT) && GetDirection() != LEFT)
        m_Direction = Direction::RIGHT;

    UpdateVelocity(); // update the velocity
    m_Position += m_Velocity * (float)ts; // update the position of the snake
    m_AABB = CalculateAABB(); // update the AABB
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
    m_AABB = CalculateAABB();
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

// helper functions, update the transformed vertices for the player object
void Player::CalculateTransformedVerts()
{
    glm::vec4 playerVertices[4] = {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f }
    };

    for (int i = 0; i < 4; i++)
    {
        m_TransformedVertices[i] = glm::translate(glm::mat4(1.0f), { m_Position.x, m_Position.y, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(GetRotation()), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.3f, 1.0f })
            * playerVertices[i];
    }
}

// calculate the AABB for the player
AABB Player::CalculateAABB()
{
    // Ensure that the transformed vertices are calculated
    CalculateTransformedVerts();

    AABB aabb;
    if(sizeof(m_TransformedVertices)/sizeof(m_TransformedVertices[0]) == 0) return aabb;

    // Initialize AABB with the first vertex
    aabb.min = aabb.max = m_TransformedVertices[0];

    // Extend the AABB with the rest of the vertices
    for (int i = 1; i < sizeof(m_TransformedVertices)/sizeof(m_TransformedVertices[0]); i++)
    {
        aabb.Encapsulate({ m_TransformedVertices[i].x, m_TransformedVertices[i].y });
    }

    return aabb;
}
