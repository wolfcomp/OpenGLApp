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
    parent = nullptr;
    for (auto child : children)
        delete child;
    if (material)
        delete material;
    if (collider)
        delete collider;
}

void Mesh::draw(glm::mat4 world_pos)
{
    if (should_draw && material != nullptr)
    {
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
    }
    for (auto child : children)
        child->draw(world_pos);
}

glm::mat4 Mesh::get_model_matrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
    return model;
}

glm::mat4 Mesh::get_world_matrix(glm::mat4 parent_pos)
{
    if (parent != nullptr)
        parent_pos = parent->get_world_matrix(parent_pos);
    return glm::scale(glm::translate(parent_pos, position) * glm::mat4_cast(rotation), scale);
}

void Mesh::draw_shadow(glm::mat4 world_pos)
{
    if (should_draw && material != nullptr && material->shadow_shader != nullptr)
    {
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        material->shadow_shader->use();
        world_pos = glm::translate(world_pos, position);
        world_pos = world_pos * glm::mat4_cast(rotation);
        world_pos = glm::scale(world_pos, scale);
        material->shadow_shader->set_mat4("model", world_pos);
        glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    }
    for (auto child : children)
        child->draw_shadow(world_pos);
}

void Mesh::set_light_space_matrix(const glm::mat4 &light_space_matrix)
{
    if (material != nullptr || material->shadow_shader != nullptr || material->shader != nullptr)
    {
        material->shadow_shader->set_mat4("lightSpaceMatrix", light_space_matrix);
        material->shader->set_mat4("lightSpaceMatrix", light_space_matrix);
    }

    for (auto child : children)
        child->set_light_space_matrix(light_space_matrix);
}

void Mesh::add_child(Mesh *child)
{
    if (child == this)
        throw "Cannot add self as child you dip shit. You want an infinite loop? :)";
    if (child->parent != nullptr)
        throw "An object can only have one parent";
    if (!can_has_children)
        throw "Object can't have children";
    child->parent = this;
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
        auto colRend = dynamic_cast<ColliderRenderIntermediate *>(child);
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

bool Mesh::is_parent(Mesh *child)
{
    if (child == this)
        return true;
    if (!parent)
        return false;
    if (child == parent)
        return true;
    return parent->is_parent(child);
}