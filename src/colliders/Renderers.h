#pragma once

#include "AABB.h"
#include "Convex.h"
#include "SphereCollider.h"
#include "../objects/Mesh.h"
#include <glad/glad.h>

struct ColliderRenderer : public Mesh
{
    void toggle()
    {
        should_draw = !should_draw;
    }

    void pre_draw() override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void post_draw() override
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
};

struct AABBRenderer : public ColliderRenderer
{
    AABB *aabb;

    AABBRenderer(AABB *aabb)
    {
        this->aabb = aabb;

        auto center = aabb->center;
        auto extent = aabb->extent;

        this->vertices = {
            Vertex(glm::vec3(-extent.x, -extent.y, -extent.z)),
            Vertex(glm::vec3(extent.x, -extent.y, -extent.z)),
            Vertex(glm::vec3(extent.x, extent.y, -extent.z)),
            Vertex(glm::vec3(-extent.x, extent.y, -extent.z)),
            Vertex(glm::vec3(-extent.x, -extent.y, extent.z)),
            Vertex(glm::vec3(extent.x, -extent.y, extent.z)),
            Vertex(glm::vec3(extent.x, extent.y, extent.z)),
            Vertex(glm::vec3(-extent.x, extent.y, extent.z))};

        this->indices = {
            0, 1, 2, 2, 3, 0,
            1, 5, 6, 6, 2, 1,
            5, 4, 7, 7, 6, 5,
            4, 0, 3, 3, 7, 4,
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4};
    }
};

struct ConvexRenderer : public ColliderRenderer
{
    ConvexHull *convex;

    ConvexRenderer(ConvexHull *convex)
    {
        this->convex = convex;

        this->vertices = convex->mesh->vertices;
        this->indices = convex->hull;
    }
};
