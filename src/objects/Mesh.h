#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
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
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    std::vector<Mesh *> children;
    GLenum mode = GL_TRIANGLES;

public:
    Mesh();
    ~Mesh();
    void draw(glm::mat4 world_pos);
    void draw_shadow(glm::mat4 world_pos);
};
