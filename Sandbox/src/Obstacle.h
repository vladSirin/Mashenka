#pragma once
#include "glm/vec2.hpp"
#include "Mashenka.h"

class Obstacle
{
public:
    enum class Type
    {
        Triangle,
        Rectangle,
        Circle,
        //other types
    };

    Obstacle(Type type, const glm::vec2& position, const glm::vec2& scale = {1.0f, 1.0f}, float rotation = 0.0f) :
        m_Type(type), m_Position(position), m_Scale(scale), m_Rotation(rotation)
    {
    }

    virtual ~Obstacle() = default;
    virtual void Init() = 0;
    virtual void LoadAsset() = 0;
    virtual bool CheckCollision(const glm::vec2& point) const = 0;
    virtual void Render() const = 0;
    virtual void Update(Mashenka::TimeStep ts) = 0;

protected:
    Type m_Type;
    glm::vec2 m_Position;
    glm::vec2 m_Scale;
    float m_Rotation;
    Mashenka::Ref<Mashenka::Texture2D> m_Texture;
};
