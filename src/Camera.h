#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

class Camera
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    void update_camera_vectors();

public:
    Camera();
    Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch);
    void set_position(const glm::vec3 position);
    void set_rotation(const float yaw, const float pitch);
    glm::mat4 get_view_matrix() const;
};
