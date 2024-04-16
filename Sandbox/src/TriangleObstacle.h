#pragma once
#include "Obstacle.h"
#include "Mashenka.h"

class TriangleObstacle : public Obstacle
{
public:
    TriangleObstacle(const glm::vec2& position, const glm::vec2& scale, float rotation, const std::vector<glm::vec2>& vertices) :
        Obstacle(Type::Triangle, position, scale, rotation), m_Vertices(vertices)
    {
    }

    void LoadAsset() override;
    void Init() override;
    bool CheckCollision(const glm::vec2& point) const override;
    void Render() const override;
    void Update(Mashenka::TimeStep ts) override;

private:
    std::vector<glm::vec2> m_Vertices; // Assuming a simple triangle

    

private:
    static bool PointInTriangle(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
};
