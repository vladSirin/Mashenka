#include "Reward.h"

#include "glm/ext/matrix_transform.hpp"

void Reward::Init()
{
    LoadAsset();
    m_AABB = CalculateAABB();
}

void Reward::LoadAsset()
{
    //Load the small rocket as a reward sprite
    m_Texture = Mashenka::Texture2D::Create("assets/textures/Snake.png");
}

void Reward::OnUpdate(Mashenka::TimeStep ts)
{
    CalculateAABB();
}

void Reward::Render() const
{
    glm::vec3 pos = {m_Position.x, m_Position.y, 0.5f};
    Mashenka::Renderer2D::DrawQuad(pos, m_Scale, m_Texture);
}

// calculate the AABB
AABB Reward::CalculateAABB()
{
    // Ensure that the transformed vertices are calculated
    CalculateTransformedVerts();

    AABB aabb;
    if (sizeof(m_TransformedVertices) / sizeof(m_TransformedVertices[0]) == 0) return aabb;

    // Initialize AABB with the first vertex
    aabb.min = aabb.max = m_TransformedVertices[0];

    // Extend the AABB with the rest of the vertices
    for (int i = 1; i < sizeof(m_TransformedVertices) / sizeof(m_TransformedVertices[0]); i++)
    {
        aabb.Encapsulate({m_TransformedVertices[i].x, m_TransformedVertices[i].y});
    }

    return aabb;
}

void Reward::CalculateTransformedVerts()
{
    glm::vec4 playerVertices[4] = {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 1.0f},
        {0.5f, 0.5f, 0.0f, 1.0f},
        {-0.5f, 0.5f, 0.0f, 1.0f}
    };

    for (int i = 0; i < 4; i++)
    {
        m_TransformedVertices[i] = glm::translate(glm::mat4(1.0f), {m_Position.x, m_Position.y, 0.0f})
            * glm::rotate(glm::mat4(1.0f), glm::radians(GetRotation()), {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {m_Scale.x, m_Scale.y, 1.0f})
            * playerVertices[i];
    }
}
