#pragma once
#include "glm/vec3.hpp"

// we are ignoring height for now
constexpr glm::vec3 reboundMultiplier = glm::vec3(1, 0, 1);

struct ICollision
{
    virtual ~ICollision() = default;
    ICollision() = default;
    ICollision(const ICollision&) = default;
    ICollision(ICollision&&) = default;
    ICollision& operator=(const ICollision&) = default;
    ICollision& operator=(ICollision&&) = default;
    glm::vec3 position = glm::vec3(0);
    void set_position(const glm::vec3& position)
    {
        this->position = position;
    }
    virtual bool contains(const glm::vec3 point) const
    {
        return false;
    }
    virtual glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const
    {
        return point;
    }
    glm::vec3 rebound_with_scalar(const glm::vec3 point, const glm::vec3 velocity) const
    {
        return rebound(point, velocity) * reboundMultiplier;
    }
    glm::vec3 rebound_with_scalar(const glm::vec3 point) const
    {
        return rebound_with_scalar(point, glm::vec3(0));
    }
    virtual void draw_bounds() {}
};

inline bool operator==(const ICollision& lhs, const glm::vec3& rhs)
{
    return lhs.contains(rhs);
}

inline bool operator==(const glm::vec3& lhs, const ICollision& rhs)
{
    return rhs.contains(lhs);
}