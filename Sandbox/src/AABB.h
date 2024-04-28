#pragma once
#include "glm/vec2.hpp"

// Creating an AABB struct for collision test
struct AABB
{
    glm::vec2 min; // bottom left
    glm::vec2 max; // top right

    AABB() : min({0.0f, 0.0f}), max({0.0f, 0.0f}) {}
    AABB(const glm::vec2& min, const glm::vec2& max) : min(min), max(max) {}

    // Expand the AABB to include a given point
    void Encapsulate(const glm::vec2& point);

    // Check if two AABBs intersect
    bool Intersects(const AABB& other) const;
};
