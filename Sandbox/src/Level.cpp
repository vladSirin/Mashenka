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
    if(CollisionTest())
    {
        GameOver();
    }
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
    //TODO: implement some GUI to start and stop the game
}

void Level::Reset()
{
    m_Player.Reset();
    m_GameOver = false;
}

bool Level::CollisionTest()
{
    for (auto& obstacle : m_Obstacles)
    {
        return m_Player.GetAABB().Intersects(obstacle.GetAABB());
    }
    return false;
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

void Level::GameOver()
{
    m_GameOver = true;
    MK_CORE_WARN("Game Over!");
}




