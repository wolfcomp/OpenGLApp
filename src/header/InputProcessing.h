#pragma once
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

public:
    InputProcessing();
    void process_keyboard(GLFWwindow* window, const double delta_time);
    void process_mouse_movement(const float x_offset, const float y_offset, const bool constrain_pitch = true);
    void process_mouse_scroll(const float y_offset);
    void set_shader(Shader *shader);
    void reset();
};
