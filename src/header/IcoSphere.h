#pragma once
#include "HSL.h"
#include "ITriangleObject.h"
#include "Math.h"

class IcoSphere : public ITriangleObject
{
    // disable linter warning for these constants as they are double to floating point conversion
    // ReSharper disable CppInconsistentNaming
    const float H_ANGLE = M_PI / 180 * 72;  // NOLINT
    const float V_ANGLE = atanf(1.0f / 2);  // NOLINT
    // ReSharper restore CppInconsistentNaming
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    hsl color;
    glm::vec3 position;
    float radius;
    glm::quat rotation;
    unsigned int subdivision = 0;
    void generate_vertices();
    void compute_half_vertex(const Vertex& a, const Vertex& b, Vertex& result) const;
    template <typename T, typename ... V>
    static void push_vector(std::vector<T>& vector, V ... args);
    template <typename T>
    static void push_vector(std::vector<T>& vector, T arg);
    template <typename T, typename ... V>
    static void push_vector(std::vector<T>& vector, T arg, V ... args);

public:
    IcoSphere();
    void set_color(const hsl& color);
    void set_position(const glm::vec3 position);
    void set_radius(const glm::vec3 radius);
    void set_subdivision(const unsigned int subdivision);
    void set_rotation(const glm::quat quaternion);
    void set_euler_rotation(const glm::vec3 angle);
    std::vector<Vertex> get_vertices() const override;
    std::vector<unsigned int> get_indices() const override;
};

