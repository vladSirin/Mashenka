#include "Level.h"

#include "Random.h"
#include "Reward.h"
#include "glm/ext/matrix_transform.hpp"


using namespace Mashenka;

void Level::Init()
{
    m_Player.LoadAssets();
    m_BackgroundManager = Ref<BackgroundManager>(new BackgroundManager(1.0f, Texture2D::Create("assets/textures/bg_space_dalle.png.")));
    Reset();
}

void Level::OnUpdate(Mashenka::TimeStep ts)
{
    // Update Player
    m_Player.OnUpdate(ts);

    // Update Background
    m_BackgroundManager->OnUpdate(m_Player.GetPosition());

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
        if (m_Rewards.size() <= MAX_REWARD_COUNT)
        {
            GenerateRewards();
        }
    }

    // Generate obstacle check
    if (m_Time - m_LastObstacleSpawnTime > m_ObstacleSpawnInterval)
    {
        m_LastObstacleSpawnTime = m_Time;
        GenerateObstacles();
    }
}

void Level::OnRender()
{
    // render background
    m_BackgroundManager->OnRender();

    // render obstacles and rewards
    for (auto& element : m_Obstacles)
    {
        // MK_CORE_INFO("Obstacle: {0}, {1}", element.GetPosition().x, element.GetPosition().y);
        element.Render();
    }
    for (auto& element : m_Rewards)
    {
        element.Render();
    }

    // render player
    m_Player.OnRender();
}

void Level::OnImGuiRender()
{
    // game hud should be handled in game layer or a hud manager
    // this is for debug only
}

void Level::Reset()
{
    m_Player.Reset();
    m_Obstacles.clear();
    m_Rewards.clear();
    m_GameOver = false;
    m_Time = 0.0f;
    m_LastRewardTime = 0.0f;
}

bool Level::ObstacleCollideTest()
{
    for (auto& obstacle : m_Obstacles)
    {
        bool collision =  m_Player.GetAABB().Intersects(obstacle.GetAABB());

        if (collision)
        {
            MK_CORE_INFO("Obstcale Collision detected!");
            return true;
        }
    }
    return false;
}

bool Level::RewardCollideTest()
{
    bool collisionDetected = false;

    for (auto it = m_Rewards.begin(); it != m_Rewards.end();)
    {
        // Check if it collided with the player
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
    
    // Raise num based on player score
    int obstacleNum = GetPlayerScore() % 3 + 1 + int(GetPlayerScore() / 3);

    // Create obstacles
    for (int i = 0; i < obstacleNum; ++i)
    {
        glm::vec3 spawnPosition = CalculateObstcaleSpawnPosition();
        CreateObstacle(spawnPosition, Random::Range(0.0f, 360.0f));
    }
    
    // clean up obstacles that are too far away from the player position
    for (auto it = m_Obstacles.begin(); it != m_Obstacles.end();)
    {
        if (abs(it->GetPosition().x - m_Player.GetPosition().x) > abs(m_CameraProjection.x) * 3.0f)
        {
            it = m_Obstacles.erase(it);
        }
        else
        {
            ++it;
        }
    }
    
}

void Level::CreateObstacle(glm::vec3 position, float angle, glm::vec2 scale)
{
    auto obstacle = Obstacle(Obstacle::Type::Triangle, position, scale, angle);
    obstacle.Init();
    m_Obstacles.emplace_back(obstacle);
}

void Level::GenerateRewards()
{
    float distanceFromPlayer = Random::Float() * 2.5f + 1.0f; 
    float angle = Random::Float() * 2.0f * glm::pi<float>();

    glm::vec2 rewardPosition = m_Player.GetPosition() + glm::vec2(distanceFromPlayer * glm::cos(angle),
                                                                  distanceFromPlayer * glm::sin(angle));

    // check if too close to any obstacle, if so retry
    for (auto& obstacle : m_Obstacles)
    {
        if (abs(obstacle.GetPosition().x - rewardPosition.x) < 1.0f && abs(obstacle.GetPosition().y - rewardPosition.y) < 1.0f)
        {
            GenerateRewards();
            return;
        }
    }
    
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

glm::vec3 Level::CalculateObstcaleSpawnPosition(float spawnMargin)
{
    glm::vec2 spawnTopLeft = {
        m_Player.GetPosition().x - m_CameraProjection.x / 2.0f - spawnMargin,
        m_Player.GetPosition().y + 1.0f + spawnMargin
    };
    glm::vec2 spawnBottomRight = {
        m_Player.GetPosition().x + m_CameraProjection.x  / 2.0f + spawnMargin,
        m_Player.GetPosition().y - 1.0f - spawnMargin
    };
    glm::vec3 spawnPosition = {
        Random::Range(spawnTopLeft.x, spawnBottomRight.x),
        Random::Range(spawnTopLeft.y, spawnBottomRight.y),
        0.5f
    };

    return spawnPosition;
}

bool Level::IsPositionOutofView(const glm::vec2& position, const glm::vec2& playerPosition, const glm::vec4& projection)
{
    // check if given position is within the player view or not
    float leftBoundary = playerPosition.x - projection[2] / 2.0f;
    float rightBoundary = playerPosition.x + projection[2] / 2.0f;
    float topBoundary = playerPosition.y + projection[3] / 2.0f;
    float bottomBoundary = playerPosition.y - projection[3] / 2.0f;
    return position.x < leftBoundary || position.x > rightBoundary || position.y < bottomBoundary || position.y > topBoundary;
    
}
