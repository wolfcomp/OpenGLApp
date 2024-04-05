#pragma once
#include "../interfaces/ITriangleObject.h"
#include "../collision/FaceCollider.h"

class Terrain : public ITriangleObject
{
    FaceCollider collider;

public:
    void generate_terrain(const char *heightmap_path, const float &height_scale, const float &texture_scale);
    FaceCollider &get_collider() { return collider; }
};