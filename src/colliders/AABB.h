#pragma once

#include <glm/vec3.hpp>
#include "Collider.h"

struct AABB : public Collider<AABB>
{
    glm::vec3 center;
    glm::vec3 extent;

    AABB() : Collider<AABB>(nullptr), center(glm::vec3(0.0f)), extent(glm::vec3(0.0f))
    {
    }

    AABB(Mesh *parent, glm::vec3 center, glm::vec3 extent) : Collider<AABB>(parent), center(center), extent(extent)
    {
    }

    bool intersects(const AABB &other) const override
    {
        auto t = other.center - center;
        auto e = other.extent + extent;

        return (abs(t.x) <= abs(e.x) && abs(t.y) <= abs(e.y) && abs(t.z) <= abs(e.z));
    }

    void rebound(const AABB &other, glm::vec3 &direction) const override
    {
        auto t = other.center - center;
        auto e = other.extent + extent;

        if (abs(t.x) <= abs(e.x) && abs(t.y) <= abs(e.y) && abs(t.z) <= abs(e.z))
        {
            direction = glm::vec3(0.0f);
            return;
        }

        auto newDirection = glm::vec3(0.0f);

        if (abs(t.x) > abs(e.x))
        {
            newDirection.x = t.x > 0 ? e.x - t.x : -e.x - t.x;
        }

        if (abs(t.y) > abs(e.y))
        {
            newDirection.y = t.y > 0 ? e.y - t.y : -e.y - t.y;
        }

        if (abs(t.z) > abs(e.z))
        {
            newDirection.z = t.z > 0 ? e.z - t.z : -e.z - t.z;
        }

        if (abs(newDirection.x) < abs(direction.x))
        {
            direction.x = newDirection.x;
        }

        if (abs(newDirection.y) < abs(direction.y))
        {
            direction.y = newDirection.y;
        }

        if (abs(newDirection.z) < abs(direction.z))
        {
            direction.z = newDirection.z;
        }
    }

    std::vector<glm::vec3> get_points() const override
    {
        auto points = std::vector<glm::vec3>();
        points.reserve(8);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    points.push_back(center + glm::vec3((i == 0 ? -1 : 1) * extent.x, (j == 0 ? -1 : 1) * extent.y, (k == 0 ? -1 : 1) * extent.z));
                }
            }
        }

        return points;
    }
};