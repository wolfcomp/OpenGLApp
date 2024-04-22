#pragma once

#include "../objects/Mesh.h"
#include <glad/glad.h>

struct ColliderRender : public Mesh
{
    ColliderRender() : Mesh()
    {
        can_has_children = false;
    }

    void pre_draw() override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void post_draw() override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

template <typename T>
struct ColliderRender : public ColliderRender
{
    T *collider;

    ColliderRender(T *collider);
};