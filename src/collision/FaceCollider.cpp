#include "FaceCollider.h"
#include "../objects/Terrain.h"

glm::vec3 FaceCollider::get_height_at_coord(const glm::vec3 &point) const
{
    auto vertices = parent->vertices;
    auto indices = parent->indices;

    for (auto i = 0; i < indices.size(); i += 3)
    {
        auto translate = glm::vec3(parent->model[3]);
        auto a = (vertices[indices[i]].position + translate).xz();
        auto b = (vertices[indices[i + 1]].position + translate).xz();
        auto c = (vertices[indices[i + 2]].position + translate).xz();

        auto p = point.xz();

        auto v0 = glm::abs(b - a);
        auto v1 = glm::abs(c - a);
        auto v2 = glm::abs(p - a);

        auto den = v0.x * v1.y - v1.x * v0.y;

        auto v = (v2.x * v1.y - v1.x * v2.y) / den;
        auto w = (v0.x * v2.y - v2.x * v0.y) / den;
        auto u = 1.0f - v - w;

        if (v >= 0 && w >= 0 && u >= 0)
        {
            auto height_a = vertices[indices[i]].position.y;
            auto height_b = vertices[indices[i + 1]].position.y;
            auto height_c = vertices[indices[i + 2]].position.y;

            return glm::vec3(point.x, height_a * u + height_b * v + height_c * w, point.z);
        }
    }

    return point;
}