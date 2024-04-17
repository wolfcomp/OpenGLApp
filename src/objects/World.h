#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Mesh.h"

class World
{
    std::vector<Mesh *> meshes;

public:
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

    void add_mesh(Mesh *mesh)
    {
        meshes.push_back(mesh);
    }

    void cleanup()
    {
        for (auto mesh : meshes)
            delete mesh;
    }
};