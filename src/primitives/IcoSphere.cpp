#include "IcoSphere.h"

IcoSphere::IcoSphere()
{
    color = hsl(0.0f, 1.0f, 0.5f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    radius = 1.0f;
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

void IcoSphere::generate_vertices()
{
    vertices.clear();
    vertices.resize(12);
    Vertex vertex;
    float h_angle1 = -M_PI / 2 - H_ANGLE / 2;
    float h_angle2 = -M_PI / 2;
    vertex.position.z = radius;
    vertex.normal = normalize(vertex.position);
    vertices[0] = vertex;
    vertex.position.z = -radius;
    vertex.normal = normalize(vertex.position);
    vertices[11] = vertex;
    for (int i = 1; i < 6; i++)
    {
        const auto i1 = i;
        const auto i2 = i + 5;

        const auto z = radius * sinf(V_ANGLE);
        const auto xy = radius * cosf(V_ANGLE);

        vertex.position.x = xy * cosf(h_angle1);
        vertex.position.y = xy * sinf(h_angle1);
        vertex.position.z = z;
        vertex.normal = normalize(vertex.position);
        vertices[i1] = vertex;

        vertex.position.x = xy * cosf(h_angle2);
        vertex.position.y = xy * sinf(h_angle2);
        vertex.position.z = -z;
        vertex.normal = normalize(vertex.position);
        vertices[i2] = vertex;

        h_angle2 += H_ANGLE;
        h_angle1 += H_ANGLE;
    }
    indices = {
        // top cone
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 1,

        // center cylinder
        6, 2, 1,
        6, 7, 2,
        7, 3, 2,
        7, 8, 3,
        8, 4, 3,
        8, 9, 4,
        9, 5, 4,
        9, 10, 5,
        10, 1, 5,
        6, 1, 10,

        // bottom cone
        11, 6, 10,
        11, 7, 6,
        11, 8, 7,
        11, 9, 8,
        11, 10, 9};

    if (subdivision > 0)
    {
        std::vector<Vertex> old_vertex;
        std::vector<unsigned> old_indices;
        unsigned index = 0;
        Vertex v1, v2, v3, new_v1, new_v2, new_v3;
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
}

void IcoSphere::compute_half_vertex(const Vertex &a, const Vertex &b, Vertex &result) const
{
    result.position = normalize(a.position + b.position);
    result.position *= radius / length(result.position);
    result.normal = normalize(result.position);
}

void IcoSphere::set_radius(const float radius)
{
    this->radius = radius;
    generate_vertices();
}

void IcoSphere::set_subdivision(const unsigned int subdivision)
{
    this->subdivision = subdivision;
    generate_vertices();
}