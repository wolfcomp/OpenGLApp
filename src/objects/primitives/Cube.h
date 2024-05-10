#pragma once

#include "../Mesh.h"
#include "../../colliders/AABB.h"
#include "../../colliders/ColliderRender.h"
#include "../../colliders/SphereCollider.h"

class Cube : public Mesh
{
public:
    Cube() : Mesh()
    {
        Vertex v0, v1, v2, v3;
        v0 = v1 = v2 = v3 = {
            glm::vec3(-1, -1, 1),
            glm::vec3(0, 0, 1),
            glm::vec2(0, 0)};

        v1.position.y = 1;
        v1.tex_coords = glm::vec2(1, 0);
        v2.position.x = 1;
        v2.position.y = -1;
        v2.tex_coords = glm::vec2(0, 1);
        v3.position.x = 1;
        v3.position.y = 1;
        v3.tex_coords = glm::vec2(1, 1);

        auto tmpVert = std::vector<Vertex>{v0, v1, v2, v3};

        auto xRot = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(1, 0, 0));
        auto yRot = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0));

        for (int i = 0; i < 4; i++)
        {
            for (auto &v : tmpVert)
            {
                v.position = glm::vec3(xRot * glm::vec4(v.position, 1));
                v.normal = glm::vec3(xRot * glm::vec4(v.normal, 0));
            }
            vertices.insert(vertices.end(), tmpVert.begin(), tmpVert.end());
        }
        for (auto &v : tmpVert)
        {
            v.position = glm::vec3(yRot * glm::vec4(v.position, 1));
            v.normal = glm::vec3(yRot * glm::vec4(v.normal, 0));
        }
        vertices.insert(vertices.end(), tmpVert.begin(), tmpVert.end());
        for (int i = 0; i < 2; i++)
        {
            for (auto &v : tmpVert)
            {
                v.position = glm::vec3(yRot * glm::vec4(v.position, 1));
                v.normal = glm::vec3(yRot * glm::vec4(v.normal, 0));
            }
        }
        vertices.insert(vertices.end(), tmpVert.begin(), tmpVert.end());
        for (int i = 0; i < 6; i++)
        {
            indices.push_back(i * 4);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 3);
            indices.push_back(i * 4 + 2);

            auto normal = -normalize(glm::cross(vertices[i * 4 + 1].position - vertices[i * 4].position,
                                                vertices[i * 4 + 2].position - vertices[i * 4].position));
            for (int j = 0; j < 4; j++)
            {
                vertices[i * 4 + j].normal = normal;
            }
        }

        auto aabb = new AABB(this, glm::vec3(0), glm::vec3(1));

        collider = aabb;

        add_child(new ColliderRender<AABB>(aabb));
    }
};