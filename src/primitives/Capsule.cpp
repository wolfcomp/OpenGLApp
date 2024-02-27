#include "Capsule.h"

Capsule::Capsule()
{
    color = hsl(0, 1, 0.5);
    position = glm::vec3(0, 0, 0);
    radius = .5f;
    height = 1;
    rotation = glm::quat(1, 0, 0, 0);
    subdivision = 0;
    generate_vertices();
}

void Capsule::generate_vertices()
{
    vertices.clear();
    indices.clear();
    vertices.reserve(10);
    Vertex vertex;
    Vertex new_v1, new_v2, new_v3, v1, v2, v3;
    float radius_mod = radius;
    vertex.color = color.get_rgb_vec3();
    vertex.position.z = radius_mod;
    vertices.push_back(vertex); // 0
    vertex.position.z = .000001f;
    vertex.position.y = radius_mod;
    vertices.push_back(vertex); // 1
    vertex.position.x = radius_mod;
    vertex.position.y = 0;
    vertices.push_back(vertex); // 2
    vertex.position.y = -radius_mod;
    vertex.position.x = 0;
    vertices.push_back(vertex); // 3
    vertex.position.x = -radius_mod;
    vertex.position.y = 0;
    vertices.push_back(vertex); // 4
    vertex.position.z = -.000001f;
    vertex.position.y = radius_mod;
    vertex.position.x = 0;
    vertices.push_back(vertex); // 5
    vertex.position.x = radius_mod;
    vertex.position.y = 0;
    vertices.push_back(vertex); // 6
    vertex.position.y = -radius_mod;
    vertex.position.x = 0;
    vertices.push_back(vertex); // 7
    vertex.position.x = -radius_mod;
    vertex.position.y = 0;
    vertices.push_back(vertex); // 8
    vertex.position.x = 0;
    vertex.position.z = -radius_mod;
    vertices.push_back(vertex); // 9

    indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 5,
        2, 6, 5,
        6, 2, 3,
        3, 7, 6,
        7, 3, 4,
        4, 8, 7,
        8, 4, 1,
        1, 5, 8,
        9, 5, 6,
        9, 6, 7,
        9, 7, 8,
        9, 8, 5
    };

    if (subdivision > 0)
    {
        std::vector<Vertex> old_vertex;
        std::vector<unsigned> old_indices;
        unsigned index = 0;
        for (unsigned i = 0; i < subdivision; ++i)
        {
            old_vertex = vertices;
            old_indices = indices;
            vertices.clear();
            indices.clear();
            index = 0;

            for (unsigned j = 0; j < old_indices.size(); j += 3)
            {
                v1 = old_vertex[old_indices[j]];
                v2 = old_vertex[old_indices[j + 1]];
                v3 = old_vertex[old_indices[j + 2]];

                compute_half_vertex(v1, v2, new_v1);
                compute_half_vertex(v2, v3, new_v2);
                compute_half_vertex(v1, v3, new_v3);

                push_vector(vertices, v1, new_v1, new_v3);
                push_vector(vertices, new_v1, v2, new_v2);
                push_vector(vertices, new_v1, new_v2, new_v3);
                push_vector(vertices, new_v3, new_v2, v3);

                for (char k = 0; k < 4; ++k)
                {
                    push_vector(indices, index, index + 1, index + 2);
                    index += 3;
                }
            }
        }
    }

    for (auto& vertex : vertices)
    {
        if (height - radius > 0)
        {
            if (vertex.position.z > 0)
                vertex.position.z += height - radius;
            else
                vertex.position.z -= height - radius;
        }

        vertex.position = rotation * vertex.position;
        vertex.position += position;
    }
}

void Capsule::compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result)
{
    result.color = color.get_rgb_vec3();
    result.position = normalize(a.position + b.position);
    result.position *= radius / length(result.position);
}

void Capsule::set_color(const hsl& color)
{
    this->color = color;
    vertices.clear();
}

void Capsule::set_euler_rotation(const glm::vec3 angle)
{
    rotation = glm::quat(angle);
    vertices.clear();
}

void Capsule::set_position(const glm::vec3 position)
{
    this->position = position;
    vertices.clear();
}

void Capsule::set_radius(const float radius)
{
    this->radius = radius;
    vertices.clear();
}

void Capsule::set_height(const float height)
{
    this->height = height;
    vertices.clear();
}

void Capsule::set_rotation(const glm::quat quaternion)
{
    rotation = quaternion;
    vertices.clear();
}

void Capsule::set_subdivision(const unsigned int subdivision)
{
    this->subdivision = subdivision;
    vertices.clear();
}

void Capsule::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
}