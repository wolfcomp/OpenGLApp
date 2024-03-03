#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "src/InputProcessing.h"
#include "src/Misc.h"
#include "src/ObjectBuffer.h"
#include "src/ShaderStore.h"
#include "src/objects/Character.h"
#include "src/objects/Door.h"
#include "src/objects/House.h"
#include "src/primitives/Cube.h"
#include "src/primitives/Plane.h"

constexpr int width = 1600;
constexpr int height = 900;
double lastX, lastY, deltaTime;
bool firstMouse = true;
int subdivision = 0;
int lastSubdivision = 0;
bool wireframe = false;
glm::vec3 staticCamPos = glm::vec3(0.f, 0.f, 3.f);
float staticCamYaw = 0.f;
float staticCamPitch = 0.f;

InputProcessing input;
ObjectBuffer objBuffer;
Character character;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    input.change_aspect(static_cast<float>(width), static_cast<float>(height));
}

void increase_subdivision()
{
    if (subdivision < 5)
    {
        subdivision++;
    }
}

void decrease_subdivision()
{
    if (subdivision > 0)
    {
        subdivision--;
    }
}

void process_mouse_input(GLFWwindow* window, const double x_pos, const double y_pos)
{
    if (firstMouse)
    {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }

    const double xOffset = x_pos - lastX;
    const double yOffset = lastY - y_pos; // reversed: y ranges bottom to top
    lastX = x_pos;
    lastY = y_pos;

    character.process_mouse_movement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    character.process_mouse_scroll(y_offset);
}

void move_character(const glm::vec3& direction)
{
    character.update_position(direction, deltaTime, objBuffer);
}

int main()
{
    input.change_aspect(width, height);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLApp", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // ReSharper disable once CppCStyleCast
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, process_mouse_input);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    input.attach_keyboard_listener(GLFW_KEY_UP, increase_subdivision, false);
    input.attach_keyboard_listener(GLFW_KEY_DOWN, decrease_subdivision, false);
    input.attach_keyboard_listener(GLFW_KEY_F, []() { wireframe = !wireframe; }, false);
    input.attach_keyboard_listener(GLFW_KEY_W, []() { move_character(glm::vec3(1, 0, 0)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_S, []() { move_character(glm::vec3(-1, 0, 0)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_A, []() { move_character(glm::vec3(0, 0, -1)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_D, []() { move_character(glm::vec3(0, 0, 1)); }, true);

    ShaderStore::add_shader("default", "shader.vs", "shader.fs");

    objBuffer.init_buffers();

    auto plane = Plane();

    plane.shader = ShaderStore::get_shader("default");

    plane.set_size(glm::vec2(100));
    plane.set_color(hsl(0, .5, .5));

    objBuffer.add_object(&plane);

    auto house = House();

    house.shader = ShaderStore::get_shader("default");
    house.set_scale(glm::vec3(2));
    house.set_position(glm::vec3(5, 0, 10));
    house.set_rotation(glm::radians(65.f));

    objBuffer.add_object(&house);

    character.set_position(glm::vec3(0, 0.75f, 0));
    character.set_shader(ShaderStore::get_shader("default"));

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        const double currentTime = glfwGetTime();
        deltaTime = (currentTime - lastTime) / 1000;
        lastTime = currentTime;
        input.process_keyboard(window, deltaTime);
        ShaderStore::set_shader_params([](const Shader* shad)
            {
                input.set_shader(shad);
                character.update_shader(shad);
            });
        if (lastSubdivision != subdivision)
        {
            lastSubdivision = subdivision;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPointSize(5);
        //enable gl wireframe mode
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        objBuffer.draw();
        character.draw();
        house.collision->draw_bounds();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
