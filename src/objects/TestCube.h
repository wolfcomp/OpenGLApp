#pragma once

#include "./primitives/Cube.h"
#include "../colliders/Collider.h"

class TestCube : public Cube
{
    float time = 0;
    float last_non_colliding_time = 0;
    bool is_colliding = false;

public:
    TestCube() : Cube()
    {
        should_update = true;
    }

    void update(float delta_time) override
    {
        if (!is_colliding)
        {
            last_non_colliding_time = time;
        }
        is_colliding = false;
        time += delta_time * 0.5f;
        time = std::fmod(time, 1.0f);
        position = glm::vec3(glm::cos(time * glm::pi<float>() * 2) * 2, 0, glm::sin(time * glm::pi<float>() * 2) * 2);
    }

    void on_collision(ColliderBase *other) override
    {
        is_colliding = true;
        position = glm::vec3(glm::cos(last_non_colliding_time * glm::pi<float>() * 2) * 2, 0, glm::sin(last_non_colliding_time * glm::pi<float>() * 2) * 2);
    }
};