#pragma once
#include "../HSL.h"
#include "../interfaces/ISphere.h"
class Cone final : public ISphere
{
    glm::vec3 position;
    glm::quat rotation;
    float radius;
    float height;
    unsigned subdivision = 0;
    hsl color;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const;
public:
    Cone();
    void set_color(const hsl& color);
    void set_position(const glm::vec3 position);
    void set_radius(const float radius);
    void set_height(const float height);
    void set_subdivision(const unsigned int subdivision);
    void set_rotation(const glm::quat quaternion);
    void set_euler_rotation(const glm::vec3 angle);
    void pre_draw() override;
};

