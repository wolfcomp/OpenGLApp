#include "Mesh.h"
#include "../Material.h"
#include "../Shader.h"
#include "../colliders/ColliderRender.h"

unsigned VAO;
unsigned VBO;
unsigned EBO;

void Mesh::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));

    glBindVertexArray(0);
}

Mesh::Mesh()
{
    position = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
    rotation = glm::quat(1, 0, 0, 0);
}

Mesh::~Mesh()
{
    for (auto child : children)
        delete child;
    if (material)
        delete material;
}

void Mesh::draw(glm::mat4 world_pos)
{
    if (!material || !should_draw)
        return;
    pre_draw();
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    material->use();
    world_pos = glm::translate(world_pos, position);
    world_pos = world_pos * glm::mat4_cast(rotation);
    world_pos = glm::scale(world_pos, scale);
    material->shader->set_mat4("model", world_pos);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    post_draw();
    for (auto child : children)
        child->draw(world_pos);
}

void Mesh::draw_shadow(glm::mat4 world_pos)
{
    if (!material || !material->shadow_shader || !should_draw)
        return;
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    material->shadow_shader->use();
    world_pos = glm::translate(world_pos, position);
    world_pos = world_pos * glm::mat4_cast(rotation);
    world_pos = glm::scale(world_pos, scale);
    material->shadow_shader->set_mat4("model", world_pos);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    for (auto child : children)
        child->draw_shadow(world_pos);
}

void Mesh::set_light_space_matrix(const glm::mat4 &light_space_matrix)
{
    if (!material || !material->shadow_shader || !material->shader)
        return;

    material->shadow_shader->set_mat4("lightSpaceMatrix", light_space_matrix);
    material->shader->set_mat4("lightSpaceMatrix", light_space_matrix);

    for (auto child : children)
        child->set_light_space_matrix(light_space_matrix);
}

void Mesh::add_child(Mesh *child)
{
    if (child == this)
        throw "Cannot add self as child you dip shit you want an infinite loop? :)";
    if (!can_has_children)
        throw "Object can't have children";
    children.push_back(child);
}

void Mesh::remove_child(Mesh *child)
{
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

void Mesh::toggle_collider_renders(bool recursive)
{
    for (auto child : children)
    {
        auto colRend = dynamic_cast<ColliderRender *>(child);
        if (colRend)
            colRend->toggle();
        else if (recursive)
            child->toggle_collider_renders(recursive);
    }
}

void Mesh::toggle()
{
    should_draw = !should_draw;
}