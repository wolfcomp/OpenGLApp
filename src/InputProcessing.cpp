#include "InputProcessing.h"

#include "glfw/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

InputProcessing::InputProcessing()
{
    zoom = 45.0f;
    aspect = 16.0f / 9.0f;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void InputProcessing::change_aspect(const float width, const float height)
{
    aspect = width / height;
    projection = glm::perspective(glm::radians(zoom), aspect, 0.1f, 100.0f);
}

void InputProcessing::set_shader(const Shader* shader)
{
    shader->set_mat4("model", value_ptr(model));
    shader->set_mat4("projection", value_ptr(projection));
}

void InputProcessing::process_keyboard(GLFWwindow* window, const double delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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