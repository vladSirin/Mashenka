#pragma once
#include "Background.h"
#include "Obstacle.h"
#include "Player.h"
#include "Reward.h"

struct SnakeObstacle
{
    glm::vec3 Position = {0.0f, 10.0f, 0.0f};
    glm::vec2 Scale = {15.0, 20.0f};
    
    
};

class Level
{
public:
    void Init();

    void OnUpdate(Mashenka::TimeStep ts);
    void OnRender();

    void OnImGuiRender();

    bool IsGameOver() const {return m_GameOver; }
    void Reset();
    bool IsPositionOutofView(const glm::vec2& position, const glm::vec2& playerPosition, const glm::vec4& projection);

    Player& GetPlayer() {return m_Player;}
    const std::vector<Reward>& GetRewards() {return m_Rewards;}
    const std::vector<Obstacle>& GetObstacles() {return m_Obstacles;}
    void SetGameTime(float time) {m_Time = time;}

    int GetPlayerScore() const {return m_Player.GetBodySegmentCount() - 1; }

private:
    bool ObstacleCollideTest();
    bool RewardCollideTest();

    void GenerateObstacles();
    void CreateObstacle(glm::vec2 position, float angle, glm::vec2 scale={1.0f, 1.0f});
    void GenerateRewards();
    void CreateReward(glm::vec2 position, glm::vec2 scale={0.2f, 0.2f});
    void GameOver();


private:
    Player m_Player;
    bool m_GameOver = false;
    std::vector<Obstacle> m_Obstacles;
    std::vector<Reward> m_Rewards;

    // Game time control
    float m_Time = 0.0f;
    
    // Reward time control
    float m_RewardSpawnInterval = 8.0f;
    float m_LastRewardTime = 0.0f;

    // obstacle time control
    float m_ObstacleSpawnInterval = 2.0f;
    float m_LastObstacleSpawnTime = 0.0f;
    glm::vec2 CalculateObstcaleSpawnPosition(float spawnMargin = 5.0f);

    // Max reward count
    static constexpr int MAX_REWARD_COUNT = 3;

    
    // background
    float m_backgroundRefreshInterval = 0.5f;
    float m_LastBackgroundRefresh = 0.0f;
    glm::vec2 m_BackgroundPosition = {0.0f, 0.0f};
    Background m_Background; // for auto-scaling background in the level
};
