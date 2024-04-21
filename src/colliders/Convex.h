#pragma once

#include <glm/vec3.hpp>
#include "Collision.h"

struct Mesh;

struct ConvexHull : public Collision<ConvexHull>
{
    std::vector<unsigend> indices;
    Mesh *mesh;
    std::vec3 position;

    ConvexHull();

    ConvexHull(Mesh *mesh);

    bool intersects(const ConvexHull &other) const;

    void rebound(const ConvexHull &other, glm::vec3 &direction) const;
};