#pragma once

#include <glm/vec3.hpp>
#include "Collider.h"
#include <vector>

struct Mesh;

struct ConvexHull : public Collider<ConvexHull>
{
    std::vector<unsigned> indices;
    Mesh *mesh;

    ConvexHull();

    ConvexHull(Mesh *mesh);

    bool intersects(const ConvexHull &other) const;

    void rebound(const ConvexHull &other, glm::vec3 &direction) const;
};