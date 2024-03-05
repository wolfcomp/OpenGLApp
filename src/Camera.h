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
    glm::vec3 eulerAngles;

    float yaw;
    float pitch;

    void update_camera_vectors();

public:
    Camera();
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    void set_position(glm::vec3 position);
    void set_rotation(glm::vec3 euler);
    glm::mat4 get_view_matrix() const;
};
