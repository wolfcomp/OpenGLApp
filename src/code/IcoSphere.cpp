#include "../header/IcoSphere.h"

IcoSphere::IcoSphere()
{
    color = hsl(0.0f, 1.0f, 0.5f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    radius = 1.0f;
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    generate_vertices();
}

void IcoSphere::generate_vertices()
{
    vertices.clear();
    vertices.resize(12);
    Vertex vertex;
    float h_angle1 = -M_PI / 2 - H_ANGLE / 2;
    float h_angle2 = -M_PI / 2;
    vertex.color = color.get_rgb_vec3();
    vertex.position.z = radius;
    vertices[0] = vertex;
    vertex.position.z = -radius;
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
        vertices[i1] = vertex;

        vertex.position.x = xy * cosf(h_angle2);
        vertex.position.y = xy * sinf(h_angle2);
        vertex.position.z = -z;
        vertices[i2] = vertex;

        h_angle2 += H_ANGLE;
        h_angle1 += H_ANGLE;
    }
    if (subdivision == 0)
    {
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
            11, 10, 9
        };
    }
    else
    {
        std::vector<Vertex> oldVertex;
        std::vector<unsigned> oldIndices;
        unsigned index = 0;
        Vertex v1, v2, v3, newV1, newV2, newV3;
        for (unsigned i = 0; i < subdivision; ++i)
        {
            oldVertex = vertices;
            oldIndices = indices;
            vertices.clear();
            indices.clear();
            index = 0;

            for (unsigned j = 0; j < oldIndices.size(); j += 3)
            {
                v1 = oldVertex[oldIndices[j]];
                v2 = oldVertex[oldIndices[j + 1]];
                v3 = oldVertex[oldIndices[j + 2]];

                compute_half_vertex(v1, v2, newV1);
                compute_half_vertex(v2, v3, newV2);
                compute_half_vertex(v1, v3, newV3);

                push_vector(vertices, v1, newV1, newV3);
                push_vector(vertices, newV1, v2, newV2);
                push_vector(vertices, newV1, newV2, newV3);
                push_vector(vertices, newV3, newV2, v3);

                for (char i = 0; i < 4; ++i)
                {
                    push_vector(indices, index, index + 1, index + 2);
                    index += 3;
                }
            }
        }
    }
}

void IcoSphere::compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const
{
    result.position = normalize(a.position + b.position);
    result.position *= radius / length(result.position);
    result.color = a.color;
}

template <typename T, typename... V>
void IcoSphere::push_vector(std::vector<T>& vector, V... args)
{
    push_vector(vector, std::forward<V>(args)...);
}

template <typename T>
void IcoSphere::push_vector(std::vector<T>& vector, T arg)
{
    vector.push_back(arg);
}

template <typename T, typename... V>
void IcoSphere::push_vector(std::vector<T>& vector, T arg, V... args)
{
    vector.push_back(arg);
    push_vector(vector, args...);
}


void IcoSphere::set_color(const hsl& color)
{
    this->color = color;
    generate_vertices();
}

void IcoSphere::set_euler_rotation(const glm::vec3 angle)
{
    rotation = glm::quat(angle);
    generate_vertices();
}

void IcoSphere::set_position(const glm::vec3 position)
{
    this->position = position;
    generate_vertices();
}

void IcoSphere::set_radius(const glm::vec3 radius)
{
    this->radius = radius.x;
    generate_vertices();
}

void IcoSphere::set_rotation(const glm::quat quaternion)
{
    rotation = quaternion;
    generate_vertices();
}

void IcoSphere::set_subdivision(const unsigned int subdivision)
{
    this->subdivision = subdivision;
    generate_vertices();
}

std::vector<unsigned> IcoSphere::get_indices() const
{
    return indices;
}

std::vector<Vertex> IcoSphere::get_vertices() const
{
    return vertices;
}