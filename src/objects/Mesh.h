#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>
#include <vector>

struct Material;
struct ColliderBase;

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
    Mesh *parent = nullptr;
    ColliderBase *collider = nullptr;
    GLenum mode = GL_TRIANGLES;
    bool should_draw = true;
    bool can_has_children = true;
    bool should_update = false;

    Mesh();
    ~Mesh();
    virtual void pre_draw() {};
    virtual void post_draw() {};
    virtual void update(float delta_time) {};
    void draw(glm::mat4 world_pos);
    void draw_shadow(glm::mat4 world_pos);
    void set_light_space_matrix(const glm::mat4 &light_space_matrix);
    glm::mat4 get_model_matrix();
    glm::mat4 get_world_matrix(glm::mat4 parent_pos = glm::mat4(1.0f));
    void add_child(Mesh *child);
    void remove_child(Mesh *child);
    void toggle_collider_renders(bool recursive = true);
    bool is_parent(Mesh *child);
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