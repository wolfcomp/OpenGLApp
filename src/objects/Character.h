#pragma once
#include "../Camera.h"
#include "../primitives/Capsule.h"
#include "../primitives/Cone.h"
#include "glm/vec3.hpp"

class ObjectBuffer;

class Character
{
    glm::vec3 position;
    float yaw;
    float pitch;
    float cameraOffset = 2.0f;
    float height = 0.75f;
    float radius = 0.25f;
    Capsule model;
    Cone look;
    Camera camera;
    ICollision* collision;
    bool cameraExplicit = false;

    void update_sub_objects();

public:
    Character();
    ~Character();
    void set_explicit_camera(const glm::vec3& position, const glm::vec3 euler, const bool was_explicit);
    void set_position(const glm::vec3& position);
    void set_look(const float yaw, const float pitch);
    void process_mouse_movement(const double x_offset, const double y_offset, const bool constrain_pitch = true);
    void process_mouse_scroll(const double y_offset);
    void set_shader(Shader* shader);
    void update_shader(const Shader* shader) const;
    void update_position(const glm::vec3& direction, const double delta_time, const ObjectBuffer& buffer);
    void check_overlap(const ObjectBuffer& buffer) const;
    void draw();
    glm::vec3 get_camera_position() const;
    glm::vec3 get_position() const;
    glm::quat get_look() const;
    glm::vec2 get_look_angles() const;
};
