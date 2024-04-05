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
        Vertex(glm::vec3(-size.x / 2, 0, -size.y / 2), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(size.x / 2, 0, -size.y / 2), glm::vec3(0, 1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(size.x / 2, 0, size.y / 2), glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(-size.x / 2, 0, size.y / 2), glm::vec3(0, 1, 0), glm::vec2(0, 1))};
    indices = {0, 1, 2, 0, 2, 3};

    for (auto i = 0; i < 2; ++i)
    {
        auto edge1 = vertices[indices[i * 3 + 1]].position - vertices[indices[i * 3]].position;
        auto edge2 = vertices[indices[i * 3 + 2]].position - vertices[indices[i * 3]].position;
        auto delta_uv1 = vertices[indices[i * 3 + 1]].texture_coord - vertices[indices[i * 3]].texture_coord;
        auto delta_uv2 = vertices[indices[i * 3 + 2]].texture_coord - vertices[indices[i * 3]].texture_coord;

        auto f = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv2.x * delta_uv1.y);

        auto tangent = glm::vec3(
            f * (delta_uv2.y * edge1.x - delta_uv1.y * edge2.x),
            f * (delta_uv2.y * edge1.y - delta_uv1.y * edge2.y),
            f * (delta_uv2.y * edge1.z - delta_uv1.y * edge2.z));
        auto bitangent = glm::vec3(
            f * (-delta_uv2.x * edge1.x + delta_uv1.x * edge2.x),
            f * (-delta_uv2.x * edge1.y + delta_uv1.x * edge2.y),
            f * (-delta_uv2.x * edge1.z + delta_uv1.x * edge2.z));

        vertices[indices[i * 3]].tangent = tangent;
        vertices[indices[i * 3 + 1]].tangent = tangent;
        vertices[indices[i * 3 + 2]].tangent = tangent;

        vertices[indices[i * 3]].bitangent = bitangent;
        vertices[indices[i * 3 + 1]].bitangent = bitangent;
        vertices[indices[i * 3 + 2]].bitangent = bitangent;
    }
}

void Plane::set_size(glm::vec2 size)
{
    this->size = size;
    generate_vertices();
}