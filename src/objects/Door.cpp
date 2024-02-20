#include "Door.h"
#include "../HSL.h"

Door::Door()
{
    position = glm::vec3(0.f, 0.f, 0.f);
    angle = 0.f;
    scale = 1.f;
    generate_vertices();
}

void Door::generate_vertices()
{
    vertices.clear();
    indices.clear();
    Vertex vertex;
    auto woodColor = hsl(30, 0.27f, .52f);
    auto ironColor = hsl(0, 0.0f, .37f);
    vertex.color = woodColor.get_rgb_vec3();
    vertex.position = glm::vec3(-0.5f, 0.f, 0.5f);
    vertices.push_back(vertex); // 0
    vertex.position = glm::vec3(-0.45f, 0.f, 0.5f);
    vertices.push_back(vertex); // 1
    vertex.position = glm::vec3(-0.45f, 1.f, 0.5f);
    vertices.push_back(vertex); // 2
    vertex.position = glm::vec3(-0.5f, 1.f, 0.5f);
    vertices.push_back(vertex); // 3
    vertex.position = glm::vec3(-0.5f, 0.f, 0.f);
    vertices.push_back(vertex); // 4
    vertex.position = glm::vec3(-0.45f, 0.f, 0.f);
    vertices.push_back(vertex); // 5
    vertex.position = glm::vec3(-0.45f, 1.f, 0.f);
    vertices.push_back(vertex); // 6
    vertex.position = glm::vec3(-0.5f, 1.f, 0.f);
    vertices.push_back(vertex); // 7
    vertex.color = ironColor.get_rgb_vec3();
    vertex.position = glm::vec3(-0.5f, 0.45f, 0.45f);
    vertices.push_back(vertex); // 8
    vertex.position = glm::vec3(-0.5f, 0.55f, 0.45f);
    vertices.push_back(vertex); // 9
    vertex.position = glm::vec3(-0.5f, 0.55f, 0.4f);
    vertices.push_back(vertex); // 10
    vertex.position = glm::vec3(-0.5f, 0.45f, 0.4f);
    vertices.push_back(vertex); // 11
    vertex.position = glm::vec3(-0.525f, 0.45f, 0.45f);
    vertices.push_back(vertex); // 12
    vertex.position = glm::vec3(-0.525f, 0.55f, 0.45f);
    vertices.push_back(vertex); // 13
    vertex.position = glm::vec3(-0.525f, 0.55f, 0.4f);
    vertices.push_back(vertex); // 14
    vertex.position = glm::vec3(-0.525f, 0.45f, 0.4f);
    vertices.push_back(vertex); // 15
    vertex.position = glm::vec3(-0.45f, 0.45f, 0.45f);
    vertices.push_back(vertex); // 16
    vertex.position = glm::vec3(-0.45f, 0.55f, 0.45f);
    vertices.push_back(vertex); // 17
    vertex.position = glm::vec3(-0.45f, 0.55f, 0.4f);
    vertices.push_back(vertex); // 18
    vertex.position = glm::vec3(-0.45f, 0.45f, 0.4f);
    vertices.push_back(vertex); // 19
    vertex.position = glm::vec3(-0.425f, 0.45f, 0.45f);
    vertices.push_back(vertex); // 20
    vertex.position = glm::vec3(-0.425f, 0.55f, 0.45f);
    vertices.push_back(vertex); // 21
    vertex.position = glm::vec3(-0.425f, 0.55f, 0.4f);
    vertices.push_back(vertex); // 22
    vertex.position = glm::vec3(-0.425f, 0.45f, 0.4f);
    vertices.push_back(vertex); // 23

    indices = {
        0, 1, 2, 2, 3, 0, // front
        4, 5, 6, 6, 7, 4, // back
        4, 5, 1, 1, 0, 4, // top
        6, 7, 3, 3, 2, 6, // bottom
        4, 1, 6, 6, 3, 1, // left
        5, 0, 7, 7, 2, 0,  // right
        8, 9, 10, 10, 11, 8, // handle right front
        12, 13, 14, 14, 15, 12, // handle right back
        8, 9, 13, 13, 12, 8, // handle right right
        10, 11, 15, 15, 14, 10, // handle right left
        8, 11, 12, 12, 15, 11, // handle right bottom
        9, 10, 13, 13, 14, 10, // handle right top
        16, 17, 18, 18, 19, 16, // handle left front
        20, 21, 22, 22, 23, 20, // handle left back
        16, 17, 21, 21, 20, 16, // handle left right
        18, 19, 23, 23, 22, 18, // handle left left
        16, 19, 20, 20, 23, 19, // handle left bottom
        17, 18, 21, 21, 22, 18  // handle left top
    };
}


void Door::set_position(const glm::vec3 position)
{
    this->position = position;
}

void Door::set_rotation(const float angle)
{
    this->angle = angle;
}

void Door::set_scale(const float scale)
{
    this->scale = scale;
}

void Door::pre_draw()
{
    if(vertices.empty())
        generate_vertices();
}
