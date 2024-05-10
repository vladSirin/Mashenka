#include "Level.h"

#include "Reward.h"
#include "glm/ext/matrix_transform.hpp"


using namespace Mashenka;

void Level::Init()
{
    m_Player.LoadAssets();
    CreateObstacle(1);
    CreateReward(1);
    Reset();
}

void Level::OnUpdate(Mashenka::TimeStep ts)
{
    m_Player.OnUpdate(ts);
    if (ObstacleCollideTest())
    {
        GameOver();
    }
    if (RewardCollideTest())
        m_Player.RewardHit();
}

void Level::OnRender()
{
    for (auto& element : m_Obstacles)
    {
        element.Render();
    }
    for (auto& element : m_Rewards)
    {
        element.Render();
    }


    m_Player.OnRender();
}

void Level::OnImGuiRender()
{
    //Most UI will be handled in GameLayer
}

void Level::Reset()
{
    m_Player.Reset();
    m_GameOver = false;
}

bool Level::ObstacleCollideTest()
{
    for (auto& obstacle : m_Obstacles)
    {
        return m_Player.GetAABB().Intersects(obstacle.GetAABB());
    }
    return false;
}

bool Level::RewardCollideTest()
{
    bool collisionDetected = false;

    for (auto it = m_Rewards.begin(); it != m_Rewards.end();)
    {
        if (m_Player.GetAABB().Intersects(it->GetAABB()))
        {

            // Destroy the intersected reward
            it = m_Rewards.erase(it);

            collisionDetected = true;
        }
        else
        {
            ++it;
        }
    }

    return collisionDetected;
}


void Level::GenerateObstacles()
{
}

void Level::CreateObstacle(int index)
{
    //TODO: now it's a temp fixed position obstacle in game middle
    const std::vector<glm::vec4>& pillarVertices = {
        {-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        {0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        {0.0f + 0.0f, 0.5f - 0.1f, 0.0f, 1.0f},
    };
    auto obstacle = Obstacle(pillarVertices, Obstacle::Type::Triangle, {0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f);
    obstacle.Init();
    m_Obstacles.emplace_back(obstacle);
}

void Level::GenerateRewards()
{
}

void Level::CreateReward(int index)
{
    //TODO: now it's a temp fixed reward to test function
    auto reward = Reward({-1.0f, -1.0f}, {0.2f, 0.2f});
    reward.Init();
    m_Rewards.emplace_back(reward);
}

void Level::GameOver()
{
    m_GameOver = true;
    MK_CORE_WARN("Game Over!");
}
