#include "Plane.h"

Plane::Plane()
{
    position = glm::vec3(0.0f);
    size = glm::vec2(1.0f);
    color = hsl(0.0f, 1.0f, 0.5f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    generate_vertices();
}

void Plane::generate_vertices()
{
    vertices = {
        Vertex(glm::vec3(-size.x / 2, 0, -size.y / 2), color.get_rgb_vec3()),
        Vertex(glm::vec3(size.x / 2, 0, -size.y / 2), color.get_rgb_vec3()),
        Vertex(glm::vec3(size.x / 2, 0, size.y / 2), color.get_rgb_vec3()),
        Vertex(glm::vec3(-size.x / 2, 0, size.y / 2), color.get_rgb_vec3())
    };
    indices = {0, 1, 2, 0, 2, 3};

    for (auto& vertex : vertices)
    {
        vertex.position = rotation * vertex.position;
        vertex.position += position;
    }
}

void Plane::set_color(const hsl& color)
{
    this->color = color;
    vertices.clear();
}

void Plane::set_position(const glm::vec3 position)
{
    this->position = position;
    vertices.clear();
}

void Plane::set_size(const glm::vec2 size)
{
    this->size = size;
    vertices.clear();
}

void Plane::set_rotation(const glm::quat quaternion)
{
    rotation = quaternion;
    vertices.clear();
}

void Plane::set_euler_rotation(const glm::vec3 angle)
{
    rotation = glm::quat(angle);
    vertices.clear();
}

void Plane::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
}
