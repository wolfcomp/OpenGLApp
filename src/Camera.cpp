#include "Camera.h"

#include "glm/gtx/quaternion.hpp"

Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0, 0)
{
}

Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch)
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
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
