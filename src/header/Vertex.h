#pragma once

#include <glm/gtc/quaternion.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture_coord;

    Vertex()
    {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        texture_coord = glm::vec2(0.0f, 0.0f);
    }

    Vertex(glm::vec3 position) : Vertex()
    {
        this->position = position;
    }

    Vertex(glm::vec3 position, glm::vec3 color) : Vertex()
    {
        this->position = position;
        this->color = color;
    }

    Vertex(glm::vec3 position, glm::vec3 color, glm::vec2 texture_coord)
    {
        this->position = position;
        this->color = color;
        this->texture_coord = texture_coord;
    }

    void rotate(glm::quat quaternion)
    {
        position = quaternion * position;
    }

    void scale(glm::vec3 scale)
    {
        position.x *= scale.x;
        position.y *= scale.y;
        position.z *= scale.z;
    }

    void translate(glm::vec3 translation)
    {
        position += translation;
    }
};