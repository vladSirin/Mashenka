#pragma once
#include "glm/vec2.hpp"

class Player {
public:
    Player();
    virtual ~Player();

    void Reset();
    void UpdateScore(int points);
    void LoseLife();
    bool IsAlive() const;
    
    // Accessors
    int GetScore() const { return m_Score; }
    int GetLives() const { return s_Lives; }
    glm::vec2 GetPosition() const {return m_Position;}

private:
    int m_Score;
    int s_Lives;
    glm::vec2 m_Position;

    // Additional player-specific attributes as needed (e.g., current level, power-ups)
};
