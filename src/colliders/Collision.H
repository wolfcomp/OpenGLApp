#pragma once

#include <glm/vec3.hpp>

template <typename T>
struct Collision
{
    virtual bool intersects(const T &other) const = 0;

    virtual void rebound(const T &other, glm::vec3 &direction) const = 0;
};