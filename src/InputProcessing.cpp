#include "InputProcessing.h"

#include "glfw/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

#define MOVEMENT_SPEED 5.f
#define MOUSE_SENSITIVITY 0.1f

InputProcessing::InputProcessing()
{
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, MOVEMENT_SPEED * 1000.0f, MOUSE_SENSITIVITY);
    zoom = 45.0f;
    aspect = 16.0f / 9.0f;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void InputProcessing::change_aspect(const float width, const float height)
{
    aspect = width / height;
    projection = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
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

    for (const auto& listener : keyboard_listeners)
    {
        auto key = listener.first;
        if (glfwGetKey(window, key) == GLFW_PRESS && (should_repeat[key] || !key_pressed[key]))
        {
            key_pressed[key] = true;
            listener.second();
        }
        else if (glfwGetKey(window, key) == GLFW_RELEASE)
        {
            key_pressed[key] = false;
        }
    }

    camera.process_keyboard(direction, delta_time);
}

void InputProcessing::attach_keyboard_listener(const int key, void (*event_handler)(), bool repeat)
{
    keyboard_listeners[key] = event_handler;
    should_repeat[key] = repeat;
    key_pressed[key] = false;
}

void InputProcessing::remove_keyboard_listener(const int key)
{
    keyboard_listeners.erase(key);
}

void InputProcessing::reset()
{
    direction = Direction::NONE;
}

void InputProcessing::process_mouse_movement(const double x_offset, const double y_offset, const bool constrain_pitch)
{
    camera.process_mouse_movement(x_offset, y_offset, constrain_pitch);
}

void InputProcessing::process_mouse_scroll(const double y_offset)
{
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom = static_cast<float>(zoom - y_offset);
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
    projection = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
}

void InputProcessing::set_camera_position(const glm::vec3 position)
{
    camera.set_position(position);
}

void InputProcessing::set_camera_rotation(const float yaw, const float pitch)
{
    camera.set_rotation(yaw, pitch);
}