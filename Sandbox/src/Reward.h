#pragma once
#include <vector>

#include "AABB.h"
#include "Mashenka.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

class Reward
{
public:
    Reward(const glm::vec2& position, const glm::vec2& scale = {1.0f, 1.0f}) :
        m_Position(position), m_Scale(scale)
    {
    }

    ~Reward() = default;

    void Init();
    void LoadAsset();
    void OnUpdate(Mashenka::TimeStep ts);
    void Render() const;
    AABB GetAABB() const { return m_AABB; }
    static float GetRotation() { return 0.0f; }

private:
    AABB m_AABB;
    glm::vec2  m_Position;
    glm::vec2  m_Scale;
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
    glm::vec4 m_TransformedVertices[4];

    AABB CalculateAABB();
    void CalculateTransformedVerts();
};
