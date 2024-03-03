#pragma once
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include <vector>

class IObject;
// we are ignoring height for now
constexpr glm::vec3 reboundMultiplier = glm::vec3(1, 0, 1);

struct ICollision
{
    virtual ~ICollision() = default;
    ICollision() = default;
    ICollision(const ICollision&) = default;
    ICollision(ICollision&&) = default;
    ICollision& operator=(const ICollision&) = default;
    ICollision& operator=(ICollision&&) = default;
    glm::vec2 position = glm::vec2(0);
    float angle = 0;
    IObject* draw_object = nullptr;
    bool should_overlap = true;
    void (*on_collision)(ICollision* self, ICollision* other) = nullptr;
    virtual glm::vec2 get_center() const
    {
        return position;
    }
    void set_position(const glm::vec2& position)
    {
        this->position = position;
    }
    void set_rotation(const float angle)
    {
        this->angle = angle;
    }
    virtual bool contains(ICollision* collider)
    {
        return false;
    }
    virtual glm::vec3 rebound(const glm::vec3 point, const glm::vec3 velocity) const
    {
        return point;
    }
    glm::vec3 rebound_with_scalar(const glm::vec3 point, const glm::vec3 velocity) const
    {
        return rebound(point, velocity) * reboundMultiplier;
    }
    glm::vec3 rebound_with_scalar(const glm::vec3 point) const
    {
        return rebound_with_scalar(point, glm::vec3(0));
    }
    virtual void draw_bounds() {}
    virtual std::vector<glm::vec2> get_points()
    {
        return { position };
    }
};