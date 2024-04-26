#include "Level.h"

#include "glm/ext/matrix_transform.hpp"


using namespace Mashenka;

void Level::Init()
{
    m_Player.LoadAssets();
    CreateObstacle(1);
    Reset();
}

void Level::OnUpdate(Mashenka::TimeStep ts)
{
    m_Player.OnUpdate(ts);
}

void Level::OnRender()
{
    for (auto& element : m_Obstacles)
    {
        element.Render();
    }

    m_Player.OnRender();
}

void Level::OnImGuiRender()
{
}

void Level::Reset()
{
    m_Player.Reset();
}

bool Level::CollisionTest()
{
    glm::vec4 playerVertices[4] = {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f }
    };

    const auto& pos = m_Player.GetPosition();
    glm::vec4 playerTransformedVerts[4];
    for (int i = 0; i < 4; i++)
    {
        playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.3f, 1.0f })
            * playerVertices[i];
    }

    //TODO: if player collides with an obstacle, game over
}

void Level::GenerateObstacles()
{
}

void Level::CreateObstacle(int index)
{
    const std::vector<glm::vec4>& pillarVertices = {
        { -0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
        {  0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f },
        {  0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f },
    };
    auto obstacle = Obstacle(pillarVertices, Obstacle::Type::Triangle, { 0.0f, 0.0f }, { 1.0f, 1.0f }, 0.0f);
    obstacle.Init();
    m_Obstacles.emplace_back(obstacle);
}




