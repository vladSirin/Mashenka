#pragma once
#include "Obstacle.h"
#include "Player.h"

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

    Player& GetPlayer() {return m_Player;}

private:
    bool CollisionTest();
    void GenerateObstacles();
    void CreateObstacle(int index);

private:
    Player m_Player;
    bool m_GameOver = false;
    std::vector<Obstacle> m_Obstacles;
};
