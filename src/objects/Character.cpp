#include "Character.h"

#include "../Math.h"
#include "../collision/AABB.h"
#include "glm/gtc/type_ptr.inl"
#include "glm/gtx/rotate_normalized_axis.inl"
#include "glm/gtx/rotate_vector.hpp"

#define MOVEMENT_SPEED 5000.f
#define MOUSE_SENSITIVITY 0.1f
#define PITCH_CONSTRAINT 25.0f

Character::Character()
{
    model = Capsule();
    look = Cone();
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    pitch = -20.0f;
    set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    model.set_subdivision(5);
    look.set_subdivision(5);
    look.set_color(hsl(188, 0.42f, 0.44f));
    model.set_color(hsl(278, 0.42f, 0.44f));
    model.set_euler_rotation(glm::vec3(glm::radians(90.f), 0, 0));
    constexpr auto height = 0.75f;
    constexpr auto radius = 0.25f;
    model.set_height(height);
    model.set_radius(radius);
    look.set_height(height / 2);
    look.set_radius(radius / 2);
    collision = new AABB(glm::vec3(-radius/2, 0, -radius/2),glm::vec3(radius/2, height/2 + radius * 1.5, radius/2));
    update_sub_objects();
}

void Character::update_sub_objects()
{
    model.set_position(position);
    const auto zRotMat = rot_z_mat(90.f);
    const auto xRotMat = rot_x_mat(-yaw);
    look.set_position(position + rotateY(glm::vec3(.45f, 0, 0), glm::radians(-yaw)) + glm::vec3(0, 0.45, 0));
    look.set_rotation(combine(&zRotMat, &xRotMat, nullptr));
    glm::vec3 cameraOffset;
    cameraOffset.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraOffset.y = sin(glm::radians(pitch));
    cameraOffset.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraOffset = normalize(cameraOffset);
    cameraOffset *= camera_offset;
    camera.set_position(position - cameraOffset + glm::vec3(0, .5, 0));
    camera.set_rotation(yaw, pitch);
    if(collision)
    {
        collision->set_position(position - position * glm::vec3(0,.5,0));
        collision->set_rotation(quat_cast(rot_y_mat(yaw)));
    }
}

void Character::set_shader(Shader* shader)
{
    model.shader = shader;
    look.shader = shader;
}

void Character::set_explicit_camera(const glm::vec3& position, const float yaw, const float pitch)
{
    camera.set_position(position);
    camera.set_rotation(yaw, pitch);
}

void Character::set_position(const glm::vec3& position)
{
    this->position = position;
    update_sub_objects();
}

void Character::set_look(const float yaw, const float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
    update_sub_objects();
}

void Character::process_mouse_movement(const double x_offset, const double y_offset, const bool constrain_pitch)
{
    yaw += static_cast<float>(x_offset * MOUSE_SENSITIVITY);
    pitch += static_cast<float>(y_offset * MOUSE_SENSITIVITY);
    if (constrain_pitch)
    {
        if (pitch > PITCH_CONSTRAINT)
            pitch = PITCH_CONSTRAINT;
        if (pitch < -PITCH_CONSTRAINT)
            pitch = -PITCH_CONSTRAINT;
    }
    update_sub_objects();
}

void Character::process_mouse_scroll(const double y_offset)
{
    camera_offset -= static_cast<float>(y_offset);
    if (camera_offset < 1.0f)
        camera_offset = 1.0f;
    if (camera_offset > 10.0f)
        camera_offset = 10.0f;

    glm::vec3 cameraOffset;
    cameraOffset.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraOffset.y = sin(glm::radians(pitch));
    cameraOffset.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraOffset = normalize(cameraOffset);
    cameraOffset *= camera_offset;
    camera.set_position(position - cameraOffset + glm::vec3(0, .5, 0));
}


void Character::update_position(const glm::vec3& direction, const double delta_time)
{
    const double velocityD = MOVEMENT_SPEED * delta_time;
    const float velocity = static_cast<float>(velocityD);
    position += rotateY(direction, glm::radians(-yaw)) * velocity;
    update_sub_objects();
}

void Character::draw()
{
    model.draw();
    look.draw();
    collision->draw_bounds();
}

glm::vec3 Character::get_position() const
{
    return position;
}

glm::quat Character::get_look() const
{
    return glm::vec3(pitch, yaw, 0);
}

void Character::update_shader(const Shader* shader) const
{
    shader->set_mat4("view", value_ptr(camera.get_view_matrix()));
}

Character::~Character()
{
    delete collision;
}

