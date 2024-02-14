#pragma once
#include <vector>

#include "HSL.h"
#include "Vertex.h"
#include "glm/vec3.hpp"
#include <glm/gtc/quaternion.hpp>

#include "ITriangleObject.h"

class Cube final : public ITriangleObject
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    hsl color;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    void generate_vertices();
    void generate_indices();

public:
    Cube();
    void set_color(const hsl color);
    void set_position(const glm::vec3 position);
    void set_scale(const glm::vec3 scale);
    void set_rotation(const glm::quat quaternion);
    void set_euler_rotation(const glm::vec3 angle);
    std::vector<Vertex> get_vertices() override;
    std::vector<unsigned int> get_indices() override;
};
