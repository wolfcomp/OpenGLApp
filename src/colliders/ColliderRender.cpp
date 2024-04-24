#include "ColliderRender.h"
#include "AABB.h"
#include "ConvexHull.h"
#include "SphereCollider.h"
#include "../Material.h"
#include "../ShaderStore.h"

ColliderRender<AABB>::ColliderRender(AABB *collider) : ColliderRenderIntermediate()
{
    this->collider = collider;

    auto center = collider->center;
    auto extent = collider->extent;

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

ColliderRender<ConvexHull>::ColliderRender(ConvexHull *collider) : ColliderRenderIntermediate()
{
    this->collider = collider;

    this->vertices = collider->mesh->vertices;
    this->indices = collider->indices;
}

ColliderRender<SphereCollider>::ColliderRender(SphereCollider *collider) : ColliderRenderIntermediate()
{
    this->collider = collider;

    auto center = collider->center;
    auto radius = collider->radius;

    auto slices = 20;
    auto stacks = 20;

    for (int i = 0; i <= stacks; i++)
    {
        float V = i / (float)stacks;
        float phi = V * glm::pi<float>();

        for (int j = 0; j <= slices; j++)
        {
            float U = j / (float)slices;
            float theta = U * (glm::pi<float>() * 2);

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            this->vertices.push_back(Vertex(glm::vec3(x * radius, y * radius, z * radius)));
        }
    }

    for (int i = 0; i < slices * stacks + slices; i++)
    {
        this->indices.push_back(i);
        this->indices.push_back(i + slices + 1);
        this->indices.push_back(i + slices);

        this->indices.push_back(i + slices + 1);
        this->indices.push_back(i);
        this->indices.push_back(i + 1);
    }
}

ColliderRenderIntermediate::ColliderRenderIntermediate() : Mesh()
{
    can_has_children = false;
    auto colorMat = new ColorMaterial();
    colorMat->color = glm::vec3(1, 0, 0);
    material = colorMat;
    material->shader = ShaderStore::get_shader("noLight");
}