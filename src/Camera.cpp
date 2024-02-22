#include "Camera.h"

#include "glm/gtx/quaternion.hpp"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = -90.0f;
    pitch = 0.0f;
    movementSpeed = 2.5f;
    mouseSensitivity = 0.1f;
    update_camera_vectors();
}

Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch, float movement_speed, float mouse_sensitivity)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->movementSpeed = movement_speed;
    this->mouseSensitivity = mouse_sensitivity;
    update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    // calculate the new front vector
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = normalize(new_front);
    // also re-calculate the right and up vector
    right = normalize(cross(front, worldUp));
    up = normalize(cross(right, front));
}

glm::mat4 Camera::get_view_matrix() const
{
    return lookAt(position, position + front, up);
}

void Camera::process_keyboard(const Direction direction, const double delta_time)
{
    if (direction == Direction::NONE)
        return;
    const double velocityD = movementSpeed * delta_time;
    const float velocity = static_cast<float>(velocityD);
    if (direction == Direction::FORWARD)
        position += front * velocity;
    if (direction == Direction::BACKWARD)
        position -= front * velocity;
    if (direction == Direction::LEFT)
        position -= right * velocity;
    if (direction == Direction::RIGHT)
        position += right * velocity;
    if (direction == Direction::UP)
        position += up * velocity;
    if (direction == Direction::DOWN)
        position -= up * velocity;
}

void Camera::process_mouse_movement(const double x_offset, const double y_offset, const bool constrain_pitch)
{
    yaw += static_cast<float>(x_offset * mouseSensitivity);
    pitch += static_cast<float>(y_offset * mouseSensitivity);
    if (constrain_pitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }
    update_camera_vectors();
}

void Camera::set_position(const glm::vec3 position)
{
    this->position = position;
}

void Camera::set_rotation(const float yaw, const float pitch)
{
    this->yaw = yaw;
    this->pitch = pitch;
    update_camera_vectors();
}