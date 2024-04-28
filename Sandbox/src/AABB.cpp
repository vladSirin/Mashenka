#include "AABB.h"

#include <algorithm>

// Expand the AABB to include a given point
void AABB::Encapsulate(const glm::vec2& point)
{
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
}


// Check if two AABBs intersect
bool AABB::Intersects(const AABB& other) const
{
    // Check if the AABBs overlap
    return !(max.x < other.min.x || min.x > other.max.x || max.y < other.min.y || min.y > other.max.y);
}
