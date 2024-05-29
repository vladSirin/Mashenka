#pragma once

#include <deque>
#include <Mashenka.h>

#include "AABB.h"

enum Direction : int
{
    UP = 180,
    DOWN = 0,
    LEFT = 90,
    RIGHT = 270,
};

class Player
{
public:
    Player();

    void LoadAssets();
    void OnUpdate(Mashenka::TimeStep ts);
    void OnRender();
    void OnImGuiRender();
    
    Direction GetDirection() const { return m_Direction; }
    float GetRotation() { return (float)GetDirection(); }
    glm::vec2 GetPosition() const { return m_Position; }
    AABB GetAABB() const { return m_AABB; }
    glm::vec2 GetVelocity() const { return m_Velocity; }
    int GetBodySegmentCount() const { return m_BodySegments.size(); }

    void Reset();
    void RewardHit();

private:
    std::vector<glm::vec2> m_BodySegments; // Store the body segments positions
    inline static constexpr float DEFAULT_DIRECTIONAL_SPEED = 3.0f;
    float m_DirectionalSpeed = DEFAULT_DIRECTIONAL_SPEED; //directional speed
    glm::vec2 m_Position = {-1.0f, 0.0f};
    glm::vec2 m_Velocity = {0.0f, 0.0f}; //this is updated based on the direction and m_Speed
    Direction m_Direction = Direction::RIGHT;
    Mashenka::Ref<Mashenka::Texture2D> m_SnakeTexture;
    AABB m_AABB; // used for collision detection
    glm::vec4 m_TransformedVertices[4]; // used for collision detection
    glm::vec2 m_Size = {0.2f, 0.2f};

    void UpdateVelocity();
    void CalculateTransformedVerts();
    AABB CalculateAABB();
    void Grow(glm::vec2 lastSegmentPos); // grow the snake

    // used for updating the body segments of the snake
    std::deque<glm::vec2> positionsQueue; // saving a queue of positions the head went based on the update delay
    int m_SegmentSpaceScale = 5;
};
