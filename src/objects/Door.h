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
    void set_position(glm::vec3 position);
    void set_scale(float scale);
    void set_rotation(float angle);
    void pre_draw() override;
};
