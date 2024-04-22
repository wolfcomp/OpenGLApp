#pragma once

#include "../Mesh.h"
#include "Math.h"
#include <glm/gtx/vector_angle.hpp>

class Arrow : public Mesh
{
    glm::vec3 direction;

public:
    Arrow(glm::vec3 direction) : Mesh()
    {
        this->direction = direction;

        double angle = std::atan2(direction.y, direction.x);
        glm::mat4 glmrotxy = glm::rotate(angle, glm::dvec3(0, 0, -1));
        double angleZ = -std::asin(direction.z);
        glm::mat4 glmrotz = glm::rotate(angleZ, glm::dvec3(-1, 0, 0));
        glm::mat4 glmrot = glmrotxy * glmrotz;
        rotation = glm::quat_cast(glmrot);

        auto zerovec3 = glm::vec3(0.0f);
        auto zerovec2 = glm::vec2(0.0f);
        vertices = {
            {{0.05f, 0.0f, 0.0f}, zerovec3, zerovec2}, // 0
            {{0.0f, 0.0f, 0.05f}, zerovec3, zerovec2}, // 1
            {{0.0f, 0.0f, -.05f}, zerovec3, zerovec2}, // 2
            {{-.05f, 0.0f, 0.0f}, zerovec3, zerovec2}, // 3
            {{0.05f, 1.0f, 0.0f}, zerovec3, zerovec2}, // 4
            {{0.0f, 1.0f, 0.05f}, zerovec3, zerovec2}, // 5
            {{0.0f, 1.0f, -.05f}, zerovec3, zerovec2}, // 6
            {{-.05f, 1.0f, 0.0f}, zerovec3, zerovec2}, // 7
            {{0.2f, 1.0f, 0.0f}, zerovec3, zerovec2},  // 8
            {{0.0f, 1.0f, 0.2f}, zerovec3, zerovec2},  // 9
            {{0.0f, 1.0f, -.2f}, zerovec3, zerovec2},  // 10
            {{-.2f, 1.0f, 0.0f}, zerovec3, zerovec2},  // 11
            {{0.0f, 1.4f, 0.0f}, zerovec3, zerovec2},  // 12
        };
        indices = {
            // base
            0,
            1,
            2,
            1,
            3,
            2,

            // shaft
            0,
            4,
            1,
            1,
            4,
            5,

            1,
            7,
            3,
            1,
            7,
            5,

            2,
            6,
            3,
            3,
            6,
            7,

            2,
            6,
            0,
            0,
            6,
            4,

            // arrow base
            8,
            9,
            10,
            9,
            11,
            10,

            // arrow head
            8,
            9,
            12,
            9,
            10,
            12,

            10,
            11,
            12,
            11,
            8,
            12,
        };
    }

    void pre_draw() override
    {
        glLineWidth(5);
    }

    void post_draw() override
    {
        glLineWidth(1);
    }
};