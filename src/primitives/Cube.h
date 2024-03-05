#pragma once
#include "../HSL.h"
#include "glm/vec3.hpp"
#include <glm/gtc/quaternion.hpp>

#include "../interfaces/ITriangleObject.h"

class Cube final : public ITriangleObject
{
    hsl color;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    void generate_vertices();
    void generate_indices();

public:
    Cube();
    void set_color(hsl color);
    void set_position(glm::vec3 position);
    void set_scale(glm::vec3 scale);
    void set_rotation(glm::quat quaternion);
    void set_euler_rotation(glm::vec3 angle);
};
