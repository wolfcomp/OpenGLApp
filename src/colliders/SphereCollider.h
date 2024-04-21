#pragma once

#include <glm/vec3.hpp>
#include "Collision.h"

struct SphereCollider : public Collision<SphereCollider>
{
    glm::vec3 center;
    float radius;

    SphereCollider() : center(glm::vec3(0.0f)), radius(0.0f)
    {
    }

    SphereCollider(glm::vec3 center, float radius) : center(center), radius(radius)
    {
    }

    bool intersects(const SphereCollider &other) const
    {
        auto t = other.center - center;
        auto r = other.radius + radius;

        return glm::dot(t, t) <= r * r;
    }

    void rebound(const SphereCollider &other, glm::vec3 &direction) const
    {
        auto t = other.center - center;
        auto r = other.radius + radius;

        if (glm::dot(t, t) <= r * r)
        {
            direction = glm::vec3(0.0f);
            return;
        }

        direction = glm::normalize(t) * (r - glm::length(t));
    }
}