#pragma once
#include "../HSL.h"
#include "../interfaces/ITriangleObject.h"
#include "../Math.h"

class IcoSphere final : public ITriangleObject
{
    // disable linter warning for these constants as they are double to floating point conversion
    // ReSharper disable CppInconsistentNaming
    const float H_ANGLE = M_PI / 180 * 72; // NOLINT
    const float V_ANGLE = atanf(1.0f / 2); // NOLINT
    // ReSharper restore CppInconsistentNaming
    hsl color;
    glm::vec3 position;
    float radius;
    glm::quat rotation;
    unsigned int subdivision = 0;
    void generate_vertices();
    void compute_half_vertex(const Vertex &a, const Vertex &b, Vertex &result) const;

public:
    IcoSphere();
    void set_color(const hsl &color);
    void set_position(glm::vec3 position);
    void set_radius(float radius);
    void set_subdivision(unsigned int subdivision);
    void set_rotation(glm::quat quaternion);
    void set_euler_rotation(glm::vec3 angle);
    void pre_draw() override;
};
