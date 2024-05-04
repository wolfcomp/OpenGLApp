#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Mesh.h"

class World
{
    std::vector<Mesh *> meshes;

public:
    void update(float delta_time)
    {
        for (auto mesh : meshes)
            mesh->update(delta_time);
    }

    void draw(glm::mat4 world_pos)
    {
        for (auto mesh : meshes)
            mesh->draw(world_pos);
    }

    void draw_shadow(glm::mat4 world_pos)
    {
        for (auto mesh : meshes)
            mesh->draw_shadow(world_pos);
    }

    void set_light_space_matrix(const glm::mat4 &light_space_matrix)
    {
        for (auto mesh : meshes)
            mesh->set_light_space_matrix(light_space_matrix);
    }

    void add_mesh(Mesh *mesh)
    {
        meshes.push_back(mesh);
    }

    void toggle_collider_render(bool recursive = true)
    {
        for (auto mesh : meshes)
            mesh->toggle_collider_renders(recursive);
    }

    void cleanup()
    {
        for (auto mesh : meshes)
            delete mesh;
    }
};