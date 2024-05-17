#include "Level.h"

#include "Random.h"
#include "Reward.h"
#include "glm/ext/matrix_transform.hpp"


using namespace Mashenka;

void Level::Init()
{
    m_Player.LoadAssets();
    m_Background.LoadAssets();
    CreateObstacle(1);
    Reset();
}

void Level::OnUpdate(Mashenka::TimeStep ts)
{
    // Update Player
    m_Player.OnUpdate(ts);

    // Check for collision
    if (ObstacleCollideTest())
    {
        GameOver();
    }
    if (RewardCollideTest())
        m_Player.RewardHit();

    // Generate reward check
    if (m_Time - m_LastRewardTime > m_RewardSpawnInterval)
    {
        m_LastRewardTime = m_Time;
        GenerateRewards();
    }
}

void Level::OnRender()
{
    m_Background.OnRender(m_Player.GetPosition());

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
    m_Time = 0.0f;
    m_LastRewardTime = 0.0f;
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
    float distanceFromPlayer = Random::Float() * 2.0f + 0.5f;
    float angle = Random::Float() * 2.0f * glm::pi<float>();

    glm::vec2 rewardPosition = m_Player.GetPosition() + glm::vec2(distanceFromPlayer * glm::cos(angle),
                                                                  distanceFromPlayer * glm::sin(angle));
    CreateReward(rewardPosition);
}

void Level::CreateReward(glm::vec2 position, glm::vec2 scale)
{
    auto reward = Reward(position, scale);
    reward.Init();
    m_Rewards.emplace_back(reward);
}

void Level::GameOver()
{
    m_GameOver = true;
    MK_CORE_WARN("Game Over!");
}
