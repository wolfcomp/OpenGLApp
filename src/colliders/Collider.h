#pragma once

#include <glm/vec3.hpp>
#include <vector>

struct ColliderBase
{
    virtual ~ColliderBase() = default;

    virtual bool intersects(const ColliderBase &other) const = 0;

    virtual void rebound(const ColliderBase &other, glm::vec3 &direction) const = 0;
};

template <typename T>
struct Collider : public ColliderBase
{
    virtual bool intersects(const T &other) const = 0;

    virtual void rebound(const T &other, glm::vec3 &direction) const = 0;

    virtual std::vector<glm::vec3> get_points() const = 0;

    bool intersects(const ColliderBase &other) const override
    {
        return intersects(static_cast<const T &>(other));
    }

    void rebound(const ColliderBase &other, glm::vec3 &direction) const override
    {
        rebound(static_cast<const T &>(other), direction);
    }

    glm::vec3 find_furthest_point(glm::vec3 direction)
    {
        auto points = get_points();
        glm::vec3 maxPoint;
        auto maxDistance = -std::numeric_limits<float>::max();
        for (auto point : points)
        {
            auto distance = dot(point, direction);
            if (distance > maxDistance)
            {
                maxDistance = distance;
                maxPoint = point;
            }
        }
        return maxPoint;
    }
};