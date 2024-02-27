#pragma once
#include "../HSL.h"
#include "../interfaces/ITriangleObject.h"
class Plane : public ITriangleObject
{
    glm::vec3 position;
    glm::vec2 size;
    hsl color;
    glm::quat rotation;
    void generate_vertices();

public:
    Plane();
    void set_color(const hsl& color);
    void set_position(const glm::vec3 position);
    void set_size(const glm::vec2 size);
    void set_rotation(const glm::quat quaternion);
    void set_euler_rotation(const glm::vec3 angle);
    void pre_draw() override;
};

