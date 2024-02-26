#pragma once
#include "ICollision.h"
#include "glm/vec3.hpp"

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
};
