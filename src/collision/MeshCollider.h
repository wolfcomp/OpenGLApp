#pragma once
#include "ICollision.h"
#include "../interfaces/ITriangleObject.h"

class MeshCollider final : ICollision
{
    ITriangleObject* mesh;
    float width;
public:
    MeshCollider(ITriangleObject* mesh, const float width) : mesh(mesh), width(width) {}
    bool contains(const glm::vec3 point) const override
    {
        glm::vec3 a0, b0, c0, a1, b1, c1, v1, v2, v3;
        bool h1, h2, h3, ret = false;
        unsigned i1, i2, i3;
        for (int i = 0; i < mesh->indices.size(); i += 3)
        {
            i1 = mesh->indices[i];
            i2 = mesh->indices[i + 1];
            i3 = mesh->indices[i + 2];
            v1 = mesh->vertices[i1].position;
            v2 = mesh->vertices[i2].position;
            v3 = mesh->vertices[i3].position;
            a0 = v1 + width;
            b0 = v2 + width;
            c0 = v3 + width;
            a1 = v1 - width;
            b1 = v2 - width;
            c1 = v3 - width;
            h1 = point.x >= a0.x && point.x <= b1.x &&
            point.y >= a0.y && point.y <= b1.y &&
            point.z >= a0.z && point.z <= b1.z;
            h2 = point.x >= b0.x && point.x <= c1.x &&
                point.y >= b0.y && point.y <= c1.y &&
                point.z >= b0.z && point.z <= c1.z;
            h3 = point.x >= c0.x && point.x <= a1.x &&
                point.y >= c0.y && point.y <= a1.y &&
                point.z >= c0.z && point.z <= a1.z;
            ret = h1 || h2 || h3 || ret;
        }
        return ret;
    }
    glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const override
    {
        glm::vec3 a0, b0, c0, a1, b1, c1, v1, v2, v3;
        bool h1, h2, h3;
        unsigned i1, i2, i3;
        for (int i = 0; i < mesh->indices.size(); i += 3)
        {
            i1 = mesh->indices[i];
            i2 = mesh->indices[i + 1];
            i3 = mesh->indices[i + 2];
            v1 = mesh->vertices[i1].position;
            v2 = mesh->vertices[i2].position;
            v3 = mesh->vertices[i3].position;
            a0 = v1 + width;
            b0 = v2 + width;
            c0 = v3 + width;
            a1 = v1 - width;
            b1 = v2 - width;
            c1 = v3 - width;
            h1 = point.x >= a0.x && point.x <= b1.x &&
            point.y >= a0.y && point.y <= b1.y &&
            point.z >= a0.z && point.z <= b1.z;
            h2 = point.x >= b0.x && point.x <= c1.x &&
                point.y >= b0.y && point.y <= c1.y &&
                point.z >= b0.z && point.z <= c1.z;
            h3 = point.x >= c0.x && point.x <= a1.x &&
                point.y >= c0.y && point.y <= a1.y &&
                point.z >= c0.z && point.z <= a1.z;
            if (h1 || h2 || h3)
                return velocity - (point - (v1 + v2 + v3) / 3.f);
        }
    }
};
