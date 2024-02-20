#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

enum class Direction
{
    NONE = 0,
    FORWARD = 1,
    BACKWARD = 2,
    LEFT = 4,
    RIGHT = 8,
    UP = 16,
    DOWN = 32
};

inline Direction operator|(Direction a, Direction b)
{
    return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));
}

inline Direction operator&(Direction a, Direction b)
{
    return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));
}

inline bool operator==(Direction a, Direction b)
{
    return static_cast<int>(a & b) != 0;
}

class Camera
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;

    float yaw;
    float pitch;

    float movement_speed;
    float mouse_sensitivity;

    void update_camera_vectors();

public:
    Camera();
    Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch, float movement_speed, float mouse_sensitivity);
    void process_keyboard(const Direction direction, const double delta_time);
    void process_mouse_movement(const double x_offset, const double y_offset, const bool constrain_pitch = true);
    void set_position(const glm::vec3 position);
    void set_rotation(const float yaw, const float pitch);
    glm::mat4 get_view_matrix() const;
};
