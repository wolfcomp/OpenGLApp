#pragma once

#include <glm/gtc/quaternion.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coord;

    Vertex()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        normal = glm::vec3(1.0f, 1.0f, 1.0f);
        texture_coord = glm::vec2(0.0f, 0.0f);
    }

    Vertex(const glm::vec3 position) : Vertex()
    {
        this->position = position;
    }

    Vertex(const glm::vec3 position, const glm::vec3 normal) : Vertex()
    {
        this->position = position;
        this->normal = normal;
    }

    Vertex(const glm::vec3 position, const glm::vec3 normal, const glm::vec2 texture_coord)
    {
        this->position = position;
        this->normal = normal;
        this->texture_coord = texture_coord;
    }

    Vertex rotate(const glm::quat quaternion)
    {
        position = quaternion * position;
        return *this;
    }

    Vertex scale(const glm::vec3 scale)
    {
        position.x *= scale.x;
        position.y *= scale.y;
        position.z *= scale.z;
        return *this;
    }

    Vertex scale(const float scale)
    {
        position.x *= scale;
        position.y *= scale;
        position.z *= scale;
        return *this;
    }

    Vertex translate(const glm::vec3 translation)
    {
        position += translation;
        return *this;
    }

    Vertex translate(const float x, const float y, const float z)
    {
        position.x += x;
        position.y += y;
        position.z += z;
        return *this;
    }
};
