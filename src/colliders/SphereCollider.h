#pragma once

#include <glm/vec3.hpp>
#include "Collider.h"

struct SphereCollider : public Collider<SphereCollider>
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

    std::vector<glm::vec3> get_points() const override
    {
        auto ret = std::vector<glm::vec3>();
        auto slices = 20;
        auto stacks = 20;

        for (int i = 0; i <= stacks; i++)
        {
            float V = i / (float)stacks;
            float phi = V * glm::pi<float>();

            for (int j = 0; j <= slices; j++)
            {
                float U = j / (float)slices;
                float theta = U * (glm::pi<float>() * 2);

                float x = cos(theta) * sin(phi);
                float y = cos(phi);
                float z = sin(theta) * sin(phi);

                ret.push_back(glm::vec3(x * radius, y * radius, z * radius));
            }
        }
        return ret;
    }
};