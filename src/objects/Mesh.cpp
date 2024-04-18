#include "Mesh.h"
#include "../Material.h"
#include "../Shader.h"

Mesh::Mesh()
{
    position = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
    rotation = glm::quat(1, 0, 0, 0);

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

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::draw(glm::mat4 world_pos)
{
    if (!material)
        return;
    glBindVertexArray(VAO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    material->use();
    world_pos = glm::translate(world_pos, position);
    world_pos = world_pos * glm::mat4_cast(rotation);
    world_pos = glm::scale(world_pos, scale);
    material->shader->set_mat4("model", world_pos);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    for (auto child : children)
        child->draw(world_pos);
}

void Mesh::draw_shadow(glm::mat4 world_pos)
{
    if (!material || !material->shadow_shader)
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