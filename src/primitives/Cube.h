#pragma once
#include "../HSL.h"
#include "glm/fwd.hpp"

#include "../interfaces/IObject.h"

class Cube final : public IObject
{
    void generate_vertices();
    void generate_indices();

public:
    Cube();
    void set_color(hsl color);
    void set_position(glm::vec3 position);
    void set_scale(glm::vec3 scale);
    void set_rotation(glm::quat quaternion);
    void set_euler_rotation(glm::vec3 angle);
    void pre_draw() override;
};
