#pragma once
#include "glm/vec3.hpp"

struct ICollision
{
    virtual ~ICollision() = default;
    ICollision() = default;
    ICollision(const ICollision&) = default;
    ICollision(ICollision&&) = default;
    ICollision& operator=(const ICollision&) = default;
    ICollision& operator=(ICollision&&) = default;
    virtual bool contains(const glm::vec3 point) const
    {
        return false;
    }
    virtual glm::vec3 rebound(const glm::vec3 point) const
    {
        return point;
    }
};

// we are ignoring height for now
constexpr glm::vec3 reboundMultiplier = glm::vec3(1,0,1);

inline bool operator==(const ICollision& lhs, const glm::vec3& rhs)
{
    return lhs.contains(rhs);
}

inline glm::vec3 operator+(const ICollision& lhs, const glm::vec3& rhs)
{
    return lhs.rebound(rhs) * reboundMultiplier;
}

inline bool operator==(const glm::vec3& lhs, const ICollision& rhs)
{
    return rhs.contains(lhs);
}

inline glm::vec3 operator+(const glm::vec3& lhs, const ICollision& rhs)
{
    return rhs.rebound(lhs) * reboundMultiplier;
}