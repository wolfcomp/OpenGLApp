#pragma once

#include <glm/vec3.hpp>

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

    bool intersects(const ColliderBase &other) const override
    {
        return intersects(static_cast<const T &>(other));
    }

    void rebound(const ColliderBase &other, glm::vec3 &direction) const override
    {
        rebound(static_cast<const T &>(other), direction);
    }
};