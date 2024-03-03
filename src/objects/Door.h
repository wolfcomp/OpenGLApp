#pragma once
#include "../interfaces/ITriangleObject.h"

class Door final : public ITriangleObject
{
    glm::vec3 position;
    float angle;
    float scale;
    void generate_vertices();

public:
    Door();
    void set_position(const glm::vec3 position);
    void set_scale(const float scale);
    void set_rotation(const float angle);
    void pre_draw() override;
};
