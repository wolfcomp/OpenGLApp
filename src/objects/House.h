#pragma once
#include "Door.h"
#include "../interfaces/ITriangleObject.h"

class House final : public ITriangleObject
{
    glm::vec3 position;
    float angle;
    float doorAngle;
    glm::vec3 scale;
    float doorOffset;
    void generate_vertices();
    Door door;

public:
    House();
    void set_position(const glm::vec3 position);
    void set_scale(const glm::vec3 scale);
    void set_rotation(const float angle);
    void set_door_offset(const float offset);
    void set_door_rotation(const float angle);
    void pre_draw() override;
};

