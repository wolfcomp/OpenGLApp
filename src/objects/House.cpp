#include "House.h"

#include "../HSL.h"
#include "glm/gtx/rotate_vector.hpp"

House::House()
{
    position = glm::vec3(0.f, 0.f, 0.f);
    angle = 0;
    scale = glm::vec3(1.f);
    doorOffset = .35f;
    generate_vertices();
}

void House::generate_vertices()
{
    Vertex vertex;
    hsl wallColor(0, .4f, .52f);
    vertex.color = wallColor.get_rgb_vec3();
    vertex.position = glm::vec3(0, 0.f, 0);
    vertices.push_back(vertex); // 0
    vertex.position = glm::vec3(0, 1.5f, 0);
    vertices.push_back(vertex); // 1
    vertex.position = glm::vec3(0, 1.5f, 4);
    vertices.push_back(vertex); // 2
    vertex.position = glm::vec3(0, 0.f, 4);
    vertices.push_back(vertex); // 3
    vertex.position.z *= doorOffset;
    vertex.position.z += 0.25f;
    vertices.push_back(vertex); // 4
    vertex.position.z -= 0.5f;
    vertices.push_back(vertex); // 5
    vertex.position.y = 1;
    vertices.push_back(vertex); // 6
    vertex.position.z += 0.5f;
    vertices.push_back(vertex); // 7
    vertex.position = glm::vec3(1.5f, 0.f, 0);
    vertices.push_back(vertex); // 8
    vertex.position = glm::vec3(1.5f, 1.5f, 0);
    vertices.push_back(vertex); // 9
    vertex.position.x /= 2;
    vertex.position.y *= 1.5f;
    vertices.push_back(vertex); // 10
    vertex.position = glm::vec3(1.5f, 0.f, 4);
    vertices.push_back(vertex); // 11
    vertex.position = glm::vec3(1.5f, 1.5f, 4);
    vertices.push_back(vertex); // 12
    vertex.position.x /= 2;
    vertex.position.y *= 1.5f;
    vertices.push_back(vertex); // 13

    indices = {
        // front
        0, 6, 1, 0, 5, 6,
        4, 3, 2, 4, 2, 7,
        1, 6, 2, 6, 7, 2,

        // left
        0, 1, 8, 8, 1, 9,
        9, 1, 10,

        // right
        3, 11, 2, 2, 11, 12,
        2, 12, 13,

        // back
        11, 8, 12, 8, 9, 12,
    };

    for (auto& vertex : vertices)
    {
        vertex.position = rotateY(vertex.position, angle);
        vertex.position *= scale;
        vertex.position += position;
    }
    vertex = vertices[5];
    door.set_position(vertex.position);
    door.set_rotation(angle + doorAngle);
}

void House::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
    door.draw();
}

void House::set_position(const glm::vec3 position)
{
    this->position = position;
    vertices.clear();
}

void House::set_rotation(const float angle)
{
    this->angle = angle;
    vertices.clear();
}

void House::set_scale(const glm::vec3 scale)
{
    this->scale = scale;
    vertices.clear();
}

void House::set_door_offset(const float offset)
{
    doorOffset = offset;
    vertices.clear();
}

void House::set_door_rotation(const float angle)
{
    doorAngle = angle;
    door.set_rotation(this->angle + doorAngle);
}
