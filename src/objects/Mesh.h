#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include <vector>

struct Material;
struct Collision;

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
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    std::vector<Mesh *> children;
    Collision *collider = nullptr;
    GLenum mode = GL_TRIANGLES;
    bool should_draw = true;
    bool can_has_children = true;

    Mesh();
    ~Mesh();
    virtual void pre_draw(){};
    virtual void post_draw(){};
    void draw(glm::mat4 world_pos);
    void draw_shadow(glm::mat4 world_pos);
    void set_light_space_matrix(const glm::mat4 &light_space_matrix);
    glm::mat4 get_model_matrix();
    void add_child(Mesh *child);
    void remove_child(Mesh *child);
    void toggle_collider_renders(bool recursive = true);
    static void setup();
    void toggle();
};

struct BasicMesh : public Mesh
{
    BasicMesh()
    {
    }

    BasicMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices)
    {
        this->vertices = vertices;
        this->indices = indices;
    }

    void pre_draw() override
    {
    }

    void post_draw() override
    {
    }
};