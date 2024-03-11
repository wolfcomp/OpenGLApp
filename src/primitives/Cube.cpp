#include "Cube.h"

Cube::Cube()
{
    color = hsl(0, 1, 0.5);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(1, 0, 0, 0);
    draw_mode = GL_TRIANGLES;
    generate_vertices();
}

void Cube::generate_indices()
{
    indices = {
        0, 1, 2, 3, 2, 1, // front
        5, 4, 6, 6, 7, 5, // back
        4, 5, 1, 1, 0, 4, // top
        7, 6, 3, 2, 3, 6, // bottom
        4, 0, 6, 2, 6, 0, // left
        1, 5, 7, 7, 3, 1 // right
    };
}


void Cube::generate_vertices()
{
    generate_indices();
    vertices.resize(8);
    Vertex v0, v1, v2, v3, v4, v5, v6, v7;
    v0 = Vertex(glm::vec3(1));
    memcpy(&v1, &v0, sizeof(Vertex));
    memcpy(&v2, &v0, sizeof(Vertex));
    memcpy(&v3, &v0, sizeof(Vertex));
    memcpy(&v4, &v0, sizeof(Vertex));
    memcpy(&v5, &v0, sizeof(Vertex));
    memcpy(&v6, &v0, sizeof(Vertex));
    memcpy(&v7, &v0, sizeof(Vertex));

    // set the correct position for each vertex
    v1.position.x = v5.position.x = -1;
    v2.position.y = v6.position.y = -1;
    v3.position.x = v7.position.x = -1;
    v3.position.y = v7.position.y = -1;

    // opposite face
    v4.position.z = v5.position.z = v6.position.z = v7.position.z = -1;

    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
    vertices[4] = v4;
    vertices[5] = v5;
    vertices[6] = v6;
    vertices[7] = v7;

    auto newVertices = std::vector<Vertex>();
    auto newIndices = std::vector<unsigned>();

    // split vertexes and calculate the normals based on the face from indices
    for (unsigned i = 0; i < indices.size(); i += 3)
    {
        auto v0 = vertices[indices[i]];
        auto v1 = vertices[indices[i + 1]];
        auto v2 = vertices[indices[i + 2]];
        auto normal = glm::normalize(glm::cross(v1.position - v0.position, v2.position - v0.position));
        v0.normal = normal;
        v1.normal = normal;
        v2.normal = normal;
        newVertices.push_back(v0);
        newVertices.push_back(v1);
        newVertices.push_back(v2);
        newIndices.push_back(i);
        newIndices.push_back(i + 1);
        newIndices.push_back(i + 2);
    }
    vertices = newVertices;
    indices = newIndices;
}


void Cube::set_color(const hsl color)
{
    this->color = color;
}

void Cube::set_position(const glm::vec3 position)
{
    this->position = position;
    compute_model_matrix();
}

void Cube::set_scale(const glm::vec3 scale)
{
    this->scale = scale;
    compute_model_matrix();
}

void Cube::set_rotation(const glm::quat quaternion)
{
    this->rotation = quaternion;
    compute_model_matrix();
}

void Cube::set_euler_rotation(const glm::vec3 angle)
{
    this->rotation = glm::quat(angle);
    compute_model_matrix();
}

void Cube::pre_draw()
{
    if (vertices.empty())
        generate_vertices();

    shader->set_vec3("albedo", color.get_rgb_vec3());
}
