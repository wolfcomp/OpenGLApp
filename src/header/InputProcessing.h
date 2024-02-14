#pragma once
#include <map>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"
#include "glfw/glfw3.h"
#include "glm/ext/matrix_clip_space.hpp"

class InputProcessing
{    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    Camera camera;
    Direction direction = Direction::NONE;
    std::map<int, void (*)()> keyboard_listeners;
    std::map<int, bool> should_repeat;
    std::map<int, bool> key_pressed;

public:
    InputProcessing();
    void set_camera_position(const glm::vec3 position);
    void set_camera_rotation(const float yaw, const float pitch);
    void change_aspect(const float width, const float height);
    void process_keyboard(GLFWwindow* window, const double delta_time);
    void process_mouse_movement(const float x_offset, const float y_offset, const bool constrain_pitch = true);
    void process_mouse_scroll(const float y_offset);
    void attach_keyboard_listener(const int key, void (*event_handler)(), bool repeat);
    void remove_keyboard_listener(const int key);
    void set_shader(Shader *shader);
    void reset();
};
