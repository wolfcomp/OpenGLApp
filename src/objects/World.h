#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Mesh.h"
#include "../colliders/Collider.h"

class World
{
    std::vector<Mesh *> meshes;

public:
    ~World()
    {
        cleanup();
    }
    void update(float delta_time)
    {
        for (auto mesh : meshes)
        {
            if (!mesh->should_update)
                continue;
            mesh->update(delta_time);
            for (auto m : get_close_meshes(mesh->position, 10, mesh))
            {
                if (m->collider && mesh->collider)
                {
                    if (mesh->collider->intersects(*m->collider))
                    {
                        mesh->collider->resolve_collision(*m->collider);
                    }
                }
            }
        }
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

    std::vector<Mesh *> get_close_meshes(glm::vec3 point, float radius, Mesh *exclude = nullptr)
    {
        std::vector<Mesh *> close_meshes;
        for (auto mesh : meshes)
        {
            if (mesh->is_parent(exclude))
                continue;
            auto distance = dot(mesh->position, point);
            if (distance < radius * radius)
                close_meshes.push_back(mesh);
        }
        return close_meshes;
    }
};