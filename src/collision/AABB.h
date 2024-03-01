#pragma once
#include "ICollision.h"
#include "../primitives/Cube.h"
#include "glm/vec3.hpp"
#include "glm/ext/vector_common.hpp"

struct AABB final : ICollision
{
    AABB() = default;
    AABB(const glm::vec3 min, const glm::vec3 max) : min(min), max(max) {}

    glm::vec3 min;
    glm::vec3 max;
    Cube bounds_object;

    bool contains(const glm::vec3 point) const override
    {
        auto curMin = min * rotation + position;
        auto curMax = max * rotation + position;
        return point.x >= curMin.x && point.x <= curMax.x &&
            point.y >= curMin.y && point.y <= curMax.y &&
            point.z >= curMin.z && point.z <= curMax.z;
    }

    glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const override
    {
        auto curMin = min + position;
        auto curMax = max + position;
        const glm::vec3 center = (curMin + curMax) / 2.f;
        const glm::vec3 half = (curMax - curMin) / 2.f;
        const glm::vec3 direction = point - center;
        glm::vec3 closest = center;
        closest.x += glm::clamp(direction.x, -half.x, half.x);
        closest.y += glm::clamp(direction.y, -half.y, half.y);
        closest.z += glm::clamp(direction.z, -half.z, half.z);
        return velocity - closest;
    }

    void draw_bounds() override
    {
        auto curMin = min + position;
        auto curMax = max + position;
        bounds_object.set_rotation(rotation);
        bounds_object.set_position((curMin + curMax) / 2.f);
        bounds_object.set_scale(max - min);
        bounds_object.draw_mode = GL_POINTS;
        bounds_object.draw();
    }

    std::vector<glm::vec3> get_points() const override
    {
        
    }
};
