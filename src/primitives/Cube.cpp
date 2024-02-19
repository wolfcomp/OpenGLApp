#include "Cube.h"

Cube::Cube()
{
    color = hsl(0, 1, 0.5);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::quat(1, 0, 0, 0);
    vertices.resize(8);
    vertices[0] = vertices[1] = vertices[2] = vertices[3] = vertices[4] = vertices[5] = vertices[6] = vertices[7] = Vertex();
    generate_vertices();
    generate_indices();
}

void Cube::generate_indices()
{
    indices = {
        0, 1, 2, 2, 3, 1, // front
        4, 5, 6, 6, 7, 5, // back
        4, 5, 1, 1, 0, 4, // top
        6, 7, 3, 3, 2, 6, // bottom
        4, 0, 6, 6, 2, 0, // left
        5, 1, 7, 7, 3, 1  // right
    };
}


void Cube::generate_vertices()
{
    Vertex v0, v1, v2, v3, v4, v5, v6, v7;
    // initialize all vertices to the same color and scale
    const auto rgb = color.get_rgb();
    v0 = Vertex(scale, glm::vec3(rgb[0], rgb[1], rgb[2]));
    memcpy(&v1, &v0, sizeof(Vertex));
    memcpy(&v2, &v0, sizeof(Vertex));
    memcpy(&v3, &v0, sizeof(Vertex));
    memcpy(&v4, &v0, sizeof(Vertex));
    memcpy(&v5, &v0, sizeof(Vertex));
    memcpy(&v6, &v0, sizeof(Vertex));
    memcpy(&v7, &v0, sizeof(Vertex));

    // set the correct position for each vertex
    v1.position.x = v5.position.x = -scale.x;
    v2.position.y = v6.position.y = -scale.y;
    v3.position.x = v7.position.x = -scale.x;
    v3.position.y = v7.position.y = -scale.y;

    // opposite face
    v4.position.z = v5.position.z = v6.position.z = v7.position.z = -scale.z;

    // rotate each vertex
    v0.rotate(rotation);
    v1.rotate(rotation);
    v2.rotate(rotation);
    v3.rotate(rotation);
    v4.rotate(rotation);
    v5.rotate(rotation);
    v6.rotate(rotation);
    v7.rotate(rotation);

    // translate each vertex to the correct position
    v0.translate(position);
    v1.translate(position);
    v2.translate(position);
    v3.translate(position);
    v4.translate(position);
    v5.translate(position);
    v6.translate(position);
    v7.translate(position);

    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;
    vertices[3] = v3;
    vertices[4] = v4;
    vertices[5] = v5;
    vertices[6] = v6;
    vertices[7] = v7;
}


void Cube::set_color(const hsl color)
{
    this->color = color;
    generate_vertices();
}

void Cube::set_position(const glm::vec3 position)
{
    this->position = position;
    generate_vertices();
}

void Cube::set_scale(const glm::vec3 scale)
{
    this->scale = scale;
    generate_vertices();
}

void Cube::set_rotation(const glm::quat quaternion)
{
    this->rotation = quaternion;
    generate_vertices();
}

void Cube::set_euler_rotation(const glm::vec3 angle)
{
    this->rotation = glm::quat(angle);
    generate_vertices();
}

