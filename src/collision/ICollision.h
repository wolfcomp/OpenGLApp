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
};
