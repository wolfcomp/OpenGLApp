#pragma once
#include "../HSL.h"
#include "../interfaces/ITriangleObject.h"
#include "../Vertex.h"

class Capsule final : public ITriangleObject
{
    hsl color;
    glm::vec3 position;
    float radius;
    float height;
    glm::quat rotation;
    unsigned int subdivision = 0;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result);
public:
    Capsule();
    void set_color(const hsl& color);
    void set_position(const glm::vec3 position);
    void set_radius(const float radius);
    void set_height(const float height);
    void set_subdivision(const unsigned int subdivision);
    void set_rotation(const glm::quat quaternion);
    void set_euler_rotation(const glm::vec3 angle);
    void pre_draw() override;
};

