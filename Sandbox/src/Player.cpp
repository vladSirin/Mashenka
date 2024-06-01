#include "Player.h"

#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui.h"

using namespace Mashenka;

Player::Player(): m_TransformedVertices{}
{
    m_BodySegments.push_back(m_Position);
    
    // Smoke
    m_SmokeParticle.Position = { 0.0f, 0.0f };
    m_SmokeParticle.Velocity = { -0.17f, 0.0f }, m_SmokeParticle.VelocityVariation = { 4.0f, 2.0f };
    m_SmokeParticle.SizeBegin = 0.05f, m_SmokeParticle.SizeEnd = 0.0f, m_SmokeParticle.SizeVariation = 0.01f;
    m_SmokeParticle.ColorBegin = { 0.8f, 0.8f, 0.8f, 1.0f };
    m_SmokeParticle.ColorEnd = { 0.6f, 0.6f, 0.6f, 1.0f };
    m_SmokeParticle.LifeTime = 4.0f;

    // Flames
    m_EngineParticle.Position = { 0.0f, 0.0f };
    m_EngineParticle.Velocity = { -0.18f, 0.0f }, m_EngineParticle.VelocityVariation = { 3.0f, 1.0f };
    m_EngineParticle.SizeBegin = 0.07f, m_EngineParticle.SizeEnd = 0.0f, m_EngineParticle.SizeVariation = 0.02f;
    m_EngineParticle.ColorBegin = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_EngineParticle.ColorEnd = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f , 1.0f };
    m_EngineParticle.LifeTime = 1.0f;
}

void Player::LoadAssets()
{
    m_SnakeTexture = Texture2D::Create("assets/textures/Snake.png");
}

void Player::OnUpdate(Mashenka::TimeStep ts)
{
    m_Time += ts;
    
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

    glm::vec2  particlePos = m_BodySegments[m_BodySegments.size() - 1];
    m_SmokeParticle.Position = particlePos;
    m_EngineParticle.Position = particlePos;
    
    // Flames
    glm::vec2 emissionPoint = { 0.0f, -0.05f };
    float rotation = glm::radians(GetRotation());
    glm::vec4 rotated = glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::vec4(emissionPoint, 0.0f, 1.0f);
    m_EngineParticle.Position = particlePos + glm::vec2{ rotated.x, rotated.y };
    m_EngineParticle.Velocity.y = -m_Velocity.y * 0.2f - 0.2f;
    m_ParticleSystem.Emit(m_EngineParticle);
    // Particles
    if (m_Time > m_SmokeNextEmitTime)
    {
        m_SmokeParticle.Position = m_Position;
        m_ParticleSystem.Emit(m_SmokeParticle);
        m_SmokeNextEmitTime += m_SmokeEmitInterval;
    }
    m_ParticleSystem.OnUpdate(ts);

    m_ParticleSystem.OnUpdate(ts);
    m_Position += m_Velocity * (float)ts; // update the position of the snake
    m_BodySegments[0] = m_Position; // update the head
    m_AABB = CalculateAABB(); // update the AABB

    // Update body segments positions based on the queued positions
    for (size_t i = 1; i < m_BodySegments.size(); ++i)
    {
        if (positionsQueue.size() > i * m_SegmentSpaceScale)
        {
            m_BodySegments[i] = positionsQueue[i *  m_SegmentSpaceScale];
        }
    }


    // Add the new head position to the front of the queue
    positionsQueue.push_front(m_BodySegments[0]);

    // Ensure the queue does not grow indefinitely
    if (positionsQueue.size() > m_BodySegments.size() * m_SegmentSpaceScale)
    {
        positionsQueue.pop_back();
    }
    
}

void Player::OnRender()
{
    m_ParticleSystem.OnRender();
    for (auto& segment : m_BodySegments)
    {
        Renderer2D::DrawRotatedQuad({segment.x, segment.y, 0.5f}, m_Size, glm::radians(GetRotation()),
                                    m_SnakeTexture);
    }
}

void Player::OnImGuiRender()
{
    ImGui::DragFloat("Speed", &m_DirectionalSpeed, 0.1f);
}

void Player::Grow(glm::vec2 lastSegmentPos)
{
    // Add a new segment at the position where the last segment was before it moved
    if (m_BodySegments.size() > 1)
    {
        glm::vec2 newSegmentPos = lastSegmentPos;
        m_BodySegments.push_back(newSegmentPos);
    }
    else
    {
        // For the first growth from just one segment, just add it directly behind the current head.
        m_BodySegments.push_back(m_Position + glm::vec2{1.0f, 0.0f});
    }
    for (auto& segment : m_BodySegments)
    {
        MK_CORE_INFO("Snake Segment: {0}, {1}", segment.x, segment.y);
    }
}

void Player::Reset()
{
    m_Position = {-3.0f, 0.0f};
    m_BodySegments.clear();
    m_BodySegments.push_back(m_Position);
    m_DirectionalSpeed = DEFAULT_DIRECTIONAL_SPEED;
    m_Direction = Direction::RIGHT;
    UpdateVelocity();
    m_AABB = CalculateAABB();
}

void Player::RewardHit()
{
    auto i = m_BodySegments.size();
    Grow(m_BodySegments[i - 1]);
}

// private functions
void Player::UpdateVelocity()
{
    switch (m_Direction)
    {
    case Direction::UP:
        m_Velocity = {0.0f, m_DirectionalSpeed * -1.0f};
        break;
    case Direction::DOWN:
        m_Velocity = {0.0f, m_DirectionalSpeed * 1.0f};
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
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f}
    };

    for (int i = 0; i < 4; i++)
    {
        m_TransformedVertices[i] = glm::translate(glm::mat4(1.0f), {m_Position.x, m_Position.y, 0.0f})
            * glm::rotate(glm::mat4(1.0f), glm::radians(GetRotation()), {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {m_Size.x, m_Size.y, 1.0f})
            * playerVertices[i];
    }
}

// calculate the AABB for the player
AABB Player::CalculateAABB()
{
    // Ensure that the transformed vertices are calculated
    CalculateTransformedVerts();

    AABB aabb;
    if (sizeof(m_TransformedVertices) / sizeof(m_TransformedVertices[0]) == 0) return aabb;

    // Initialize AABB with the first vertex
    aabb.min = aabb.max = m_TransformedVertices[0];

    // Extend the AABB with the rest of the vertices
    for (int i = 1; i < sizeof(m_TransformedVertices) / sizeof(m_TransformedVertices[0]); i++)
    {
        aabb.Encapsulate({m_TransformedVertices[i].x, m_TransformedVertices[i].y});
    }

    return aabb;
}
