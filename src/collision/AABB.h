#pragma once
#include "ICollision.h"
#include "glm/vec3.hpp"
#include "glm/ext/vector_common.hpp"

struct AABB final : ICollision
{
    glm::vec3 min;
    glm::vec3 max;

    bool contains(const glm::vec3 point) const override
    {
        return point.x >= min.x && point.x <= max.x &&
            point.y >= min.y && point.y <= max.y &&
            point.z >= min.z && point.z <= max.z;
    }

    glm::vec3 rebound(const glm::vec3 point) const override
    {
        const glm::vec3 center = (min + max) / 2.f;
        const glm::vec3 half = (max - min) / 2.f;
        const glm::vec3 direction = point - center;
        glm::vec3 closest = center;
        closest.x += glm::clamp(direction.x, -half.x, half.x);
        closest.y += glm::clamp(direction.y, -half.y, half.y);
        closest.z += glm::clamp(direction.z, -half.z, half.z);
        return closest;
    }
};
