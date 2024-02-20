#include "Door.h"
#include "../HSL.h"
#include "glm/gtx/rotate_vector.hpp"

Door::Door()
{
    position = glm::vec3(0.f, 0.f, 0.f);
    angle = 0.f;
    scale = 1.f;
    generate_vertices();
}

std::vector<unsigned> get_handle(unsigned index)
{
    return {
        index, index + 1, index + 2, index + 2, index + 3, index, // front
        index + 4, index + 5, index + 6, index + 6, index + 7, index + 4, // back
        index, index + 1, index + 5, index + 5, index + 4, index, // right
        index + 2, index + 3, index + 7, index + 7, index + 6, index + 2, // left
        index, index + 3, index + 4, index + 4, index + 7, index + 3, // bottom
        index + 1, index + 2, index + 5, index + 5, index + 6, index + 2, // top
    };
};

void Door::generate_vertices()
{
    vertices.clear();
    indices.clear();
    Vertex vertex;
    auto woodColor = hsl(30, 0.27f, .52f);
    auto darkWoodColor = hsl(30, 0.27f, .37f);
    auto ironColor = hsl(0, 0.0f, .37f);
    unsigned handleRight, handleLeft;
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
    auto inset = 0.02f;
    auto vecInset = glm::vec3(0.f, inset, -inset);
    vertex = vertices[0];
    vertices.push_back(vertex.translate(vecInset)); // 8
    vertex = vertices[1];
    vertices.push_back(vertex.translate(vecInset)); // 9
    vecInset = glm::vec3(0.f, -inset, -inset);
    vertex = vertices[2];
    vertices.push_back(vertex.translate(vecInset)); // 10
    vertex = vertices[3];
    vertices.push_back(vertex.translate(vecInset)); // 11
    vecInset = glm::vec3(0.f, inset, inset);
    vertex = vertices[4];
    vertices.push_back(vertex.translate(vecInset)); // 12
    vertex = vertices[5];
    vertices.push_back(vertex.translate(vecInset)); // 13
    vecInset = glm::vec3(0.f, -inset, inset);
    vertex = vertices[6];
    vertices.push_back(vertex.translate(vecInset)); // 14
    vertex = vertices[7];
    vertices.push_back(vertex.translate(vecInset)); // 15
    vertex = vertices[8];
    inset = 0.015f;
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(inset, 0, 0)); // 16
    vertex = vertices[9];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(-inset, 0, 0)); // 17
    vertex = vertices[10];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(-inset, 0, 0)); // 18
    vertex = vertices[11];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(inset, 0, 0)); // 19
    vertex = vertices[12];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(inset, 0, 0)); // 20
    vertex = vertices[13];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(-inset, 0, 0)); // 21
    vertex = vertices[14];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(-inset, 0, 0)); // 22
    vertex = vertices[15];
    vertex.color = darkWoodColor.get_rgb_vec3();
    vertices.push_back(vertex.translate(inset, 0, 0)); // 23

    // handles
    vertex.color = ironColor.get_rgb_vec3();
    handleRight = vertices.size();
    vertex.position = glm::vec3(-0.5f, 0.45f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.5f, 0.55f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.5f, 0.55f, 0.4f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.5f, 0.45f, 0.4f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.525f, 0.45f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.525f, 0.55f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.525f, 0.55f, 0.4f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.525f, 0.45f, 0.4f);
    vertices.push_back(vertex);
    handleLeft = vertices.size();
    vertex.position = glm::vec3(-0.45f, 0.45f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.45f, 0.55f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.45f, 0.55f, 0.4f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.45f, 0.45f, 0.4f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.425f, 0.45f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.425f, 0.55f, 0.45f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.425f, 0.55f, 0.4f);
    vertices.push_back(vertex);
    vertex.position = glm::vec3(-0.425f, 0.45f, 0.4f);
    vertices.push_back(vertex);

    for (int i = handleRight; i < handleLeft; i++)
    {
        vertices[i].translate(inset, 0, 0);
    }
    for (int i = handleLeft; i < vertices.size(); i++)
    {
        vertices[i].translate(-inset, 0, 0);
    }

    indices = {
        0, 1, 2, 2, 3, 0, // left
        4, 5, 6, 6, 7, 4, // right
        0, 1, 5, 5, 4, 0, // bottom
        2, 3, 7, 7, 6, 2, // top
        0, 8, 11, 11, 3, 0, // front right
        4, 0, 8, 8, 12, 4, // front bottom
        3, 7, 15, 15, 11, 3, // front top
        7, 4, 12, 12, 15, 7, // front left
        1, 5, 13, 13, 9, 1, // back bottom
        1, 2, 10, 10, 9, 1, // back top
        2, 6, 14, 14, 10, 2, // back right
        5, 13, 14, 14, 6, 5, // back left

        // inset front
        8, 11, 19, 19, 16, 8, // right
        12, 8, 16, 16, 20, 12, // bottom
        15, 12, 20, 20, 23, 15, // left
        11, 15, 23, 23, 19, 11, // top
        16, 19, 23, 23, 20, 16, // front

        // inset back
        9, 10, 18, 18, 17, 9, // right
        13, 9, 17, 17, 21, 13, // bottom
        14, 13, 21, 21, 22, 14, // left
        10, 14, 22, 22, 18, 10, // top
        17, 18, 22, 22, 21, 17, // back
    };

    auto handleRightIndices = get_handle(handleRight);
    auto handleLeftIndices = get_handle(handleLeft);
    indices.insert(indices.end(), handleRightIndices.begin(), handleRightIndices.end());
    indices.insert(indices.end(), handleLeftIndices.begin(), handleLeftIndices.end());

    for (auto& vertex : vertices)
    {
        vertex.position *= scale;
        vertex.position = rotateY(vertex.position, angle);
        vertex.position += position;
    }
}


void Door::set_position(const glm::vec3 position)
{
    this->position = position;
    vertices.clear();
}

void Door::set_rotation(const float angle)
{
    this->angle = angle;
    vertices.clear();
}

void Door::set_scale(const float scale)
{
    this->scale = scale;
    vertices.clear();
}

void Door::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
}
