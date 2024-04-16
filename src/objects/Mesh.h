#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Material;

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material *material;
    unsigned int VAO, VBO, EBO;

public:
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material *material);
    ~Mesh();
    void draw(GLenum mode = GL_TRIANGLES);
};
