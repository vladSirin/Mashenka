#pragma once
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

    Obstacle(const std::vector<glm::vec4>& vertices, Type type, const glm::vec2& position, const glm::vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f) :
        m_Type(type), m_Position(position), m_Scale(scale), m_Rotation(rotation), m_Vertices(vertices)
    {
    }
    

     ~Obstacle() = default;
    void Init();
    void LoadAsset();
    bool CheckPointCollision(const glm::vec2& point);
    void Render() const;
    void Update(Mashenka::TimeStep ts);
    Type GetType() const {return m_Type;}

private:
    Type m_Type;
    glm::vec2 m_Position;
    glm::vec2 m_Scale;
    float m_Rotation;
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
    std::vector<glm::vec4> m_Vertices;

private:
    bool PointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
};
