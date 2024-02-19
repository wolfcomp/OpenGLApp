#pragma once
#include "../HSL.h"
#include "../interfaces/ISphere.h"
#include "../Vertex.h"

class Sphere final :  public ISphere
{
    hsl color;
    glm::vec3 position;
    float radius;
    glm::quat rotation;
    unsigned int subdivision = 0;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const;
public:
    Sphere();
    void set_color(const hsl& color);
    void set_position(const glm::vec3 position);
    void set_radius(const float radius);
    void set_subdivision(const unsigned int subdivision);
    void set_rotation(const glm::quat quaternion);
    void set_euler_rotation(const glm::vec3 angle);
    void pre_draw() override;
};

