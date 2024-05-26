#pragma once
#include "AABB.h"
#include "glm/vec2.hpp"
#include "Mashenka.h"

class Obstacle
{
public:
    enum class Type
    {
        Triangle = 0,
        Rectangle = 1,
        Circle = 2,
        //other types
    };

    Obstacle(Type type, const glm::vec3& position, const glm::vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f) :
        m_Type(type), m_Position(position), m_Scale(scale), m_Rotation(rotation)
    {
    }
    

     ~Obstacle() = default;
    void Init();
    void LoadAsset();
    bool CheckPointCollision(const glm::vec2& point);
    void Render() const;
    void Update(Mashenka::TimeStep ts);
    Type GetType() const {return m_Type;}
    AABB GetAABB() const { return m_AABB; }

    glm::vec2 GetPosition() const {return m_Position;}
    glm::vec2 GetScale() const {return m_Scale;}
    float GetRotation() const {return m_Rotation;}
    Mashenka::Ref<Mashenka::Texture2D> GetTexture() const {return m_Texture;}

private:
    Type m_Type;
    glm::vec3 m_Position;
    glm::vec2 m_Scale;
    float m_Rotation;
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
    glm::vec4 m_Vertices[3];
    AABB m_AABB;

private:
    AABB CalculateAABB();
    bool PointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
    void CalculateTransformedVerts();
};
