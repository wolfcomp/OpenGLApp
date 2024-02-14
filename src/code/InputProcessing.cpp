#include "../header/InputProcessing.h"

#include "glfw/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

InputProcessing::InputProcessing()
{
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 25000.0f, 0.01f);
}

void InputProcessing::set_shader(Shader* shader)
{
    shader->set_mat4("view", value_ptr(camera.get_view_matrix()));
    shader->set_mat4("model", value_ptr(model));
    shader->set_mat4("projection", value_ptr(projection));
}

void InputProcessing::process_keyboard(GLFWwindow* window, const double delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction = direction | Direction::FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction = direction | Direction::BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction = direction | Direction::LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction = direction | Direction::RIGHT;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        direction = direction | Direction::UP;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        direction = direction | Direction::DOWN;

    camera.process_keyboard(direction, delta_time);
}

void InputProcessing::reset()
{
    direction = Direction::NONE;
}

void InputProcessing::process_mouse_movement(const float x_offset, const float y_offset, const bool constrain_pitch)
{
    camera.process_mouse_movement(x_offset, y_offset, constrain_pitch);
}