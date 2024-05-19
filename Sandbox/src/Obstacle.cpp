#pragma once

#include "Obstacle.h"

#include "glm/ext/matrix_transform.hpp"

void Obstacle::Init()
{
    LoadAsset();
    m_AABB = CalculateAABB();
}

void Obstacle::LoadAsset()
{
    // Load the triangle
    switch (m_Type)
    {
    case Type::Triangle:
        m_Texture = Mashenka::Texture2D::Create("assets/textures/Triangle.png");
        break;
    case Type::Rectangle:
        break;
    case Type::Circle:
        break;
    }
}

bool Obstacle::CheckPointCollision(const glm::vec2& point)
{
    switch (m_Type)
    {
        case Type::Triangle:
            return PointInTriangle(point, m_Vertices[0], m_Vertices[1], m_Vertices[2]);
        default:
            return false;
    }
}

void Obstacle::Render() const
{
    // Render the triangle
    Mashenka::Renderer2D::DrawRotatedQuad(m_Position, m_Scale, m_Rotation, m_Texture);
}

void Obstacle::Update(Mashenka::TimeStep ts)
{
    CalculateAABB();
}


// private
AABB Obstacle::CalculateAABB()
{
    // Calculate the AABB of the obstacle
    CalculateTransformedVerts();
    
    AABB aabb;
    if (sizeof(m_Vertices) / sizeof(m_Vertices[0]) == 0) return aabb;

    // Initialize AABB with the first vertex
    aabb.min = aabb.max = m_Vertices[0];

    // Extend the AABB with the rest of the vertices
    for (int i = 1; i < sizeof(m_Vertices) / sizeof(m_Vertices[0]); i++)
    {
        aabb.Encapsulate({m_Vertices[i].x, m_Vertices[i].y});
    }

    return aabb;
}

bool Obstacle::PointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    // Calculate the area of the original triangle
    float areaOrig = glm::abs((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));

    // Calculate the areas of three triangles formed by the point and the vertices of the triangle
    float area1 = glm::abs((a.x - p.x) * (b.y - p.y) - (a.y - p.y) * (b.x - p.x));
    float area2 = glm::abs((b.x - p.x) * (c.y - p.y) - (b.y - p.y) * (c.x - p.x));
    float area3 = glm::abs((c.x - p.x) * (a.y - p.y) - (c.y - p.y) * (a.x - p.x));

    // If the sum of the areas of the three triangles is equal to the area of the original triangle, the point is inside the triangle
    return (area1 + area2 + area3 == areaOrig);
}

void Obstacle::CalculateTransformedVerts()
{
    // Pillar vertices at 0,0
    const std::vector<glm::vec4>& pillarVertices = {
        {-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        {0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        {0.0f + 0.0f, 0.5f - 0.1f, 0.0f, 1.0f},
    };
    
    // translate the vertices based on the position
    for (int i = 0; i < 3; i++)
    {
        m_Vertices[i] = glm::translate(glm::mat4(1.0f), {m_Position.x, m_Position.y, 0.0f})
            * glm::rotate(glm::mat4(1.0f), glm::radians(GetRotation()), {0.0f, 0.0f, 1.0f})
            * glm::scale(glm::mat4(1.0f), {m_Scale.x, m_Scale.y, 1.0f})
            * pillarVertices[i];
    }
}


