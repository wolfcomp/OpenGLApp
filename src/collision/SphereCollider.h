#pragma once
#include "ICollision.h"
#include "glm/detail/func_geometric.inl"

struct SphereCollider final : ICollision
{
    float radius;
    glm::vec3 center;

    bool contains(const glm::vec3 point) const override
    {
        return abs(distance(point, center)) <= radius;
    }

    glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const override
    {
        return velocity - center + radius * normalize(point - center);
    }
};
