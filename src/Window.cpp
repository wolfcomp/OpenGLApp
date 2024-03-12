#include "Window.h"
#include <iostream>
#include "TimeManager.h"
#include "ObjectBuffer.h"
#include "objects/Character.h"
#include "ShaderStore.h"
#include "primitives/Cube.h"
#include "InputProcessing.h"

constexpr int width = 1600;
constexpr int height = 900;
double lastX, lastY;
bool firstMouse = true;
int subdivision = 0;
int lastSubdivision = 0;
bool wireframe = false;
glm::vec3 staticCameraPos = glm::vec3(5.5, 3, 5);
float staticCamYaw = 60.f;
float staticCamPitch = -25.f;
float curveAggressiveness = 1;
bool lastCharacterStateObserved = false;
float prevYawExplicit;
float prevPitchExplicit;
hsl skyColor = hsl(180, .85f, .75);
hsl ambientColor = hsl(0, 1, .4f);
glm::vec3 lightPos = glm::vec3(-10, 10, -10);

InputProcessing input;
ObjectBuffer objBuffer;
Character character;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    input.change_aspect(static_cast<float>(width), static_cast<float>(height));
}

void process_mouse_input(GLFWwindow *window, const double x_pos, const double y_pos)
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

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    character.process_mouse_scroll(y_offset);
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

void move_character(const glm::vec3 &direction)
{
    character.update_position(direction, TimeManager::get_delta_time(), objBuffer);
}

int Window::init()
{
    input.change_aspect(width, height);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(width, height, "OpenGLApp", nullptr, nullptr);
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
    input.attach_keyboard_listener(
        GLFW_KEY_F, []()
        { wireframe = !wireframe; },
        false);
    input.attach_keyboard_listener(
        GLFW_KEY_W, []()
        { move_character(glm::vec3(1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_S, []()
        { move_character(glm::vec3(-1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_A, []()
        { move_character(glm::vec3(0, 0, -1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_D, []()
        { move_character(glm::vec3(0, 0, 1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_SPACE, []()
        { move_character(glm::vec3(0, 1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_LEFT_CONTROL, []()
        { move_character(glm::vec3(0, -1, 0)); },
        true);
}

void Window::create_objects()
{
    objBuffer.init_buffers();

    auto cube = new Cube();

    cube->set_position(glm::vec3(3, 3, 3));
    cube->set_scale(glm::vec3(3, 3, 3));
    cube->set_shader(ShaderStore::get_shader("uvVisual"));

    objBuffer.add_object(cube);

    auto cube2 = new Cube();

    cube2->set_position(lightPos);
    cube2->set_scale(glm::vec3(.1f, .1f, .1f));
    cube2->set_shader(ShaderStore::get_shader("default"));

    objBuffer.add_object(cube2);

    character.set_position(glm::vec3(-5, 0.75f, 0));
    character.set_shader(ShaderStore::get_shader("default"));
}

void Window::update()
{
    TimeManager::set_current_frame(glfwGetTime());
    TimeManager::set_delta_time((TimeManager::get_current_frame() - TimeManager::get_last_frame()) / 1000);
    TimeManager::set_last_frame(TimeManager::get_current_frame());
    input.process_keyboard(window, TimeManager::get_delta_time());
    ShaderStore::set_shader_params([](const Shader *shad)
                                   {
                shad->set_vec3("ambientColor", ambientColor.get_rgb_vec3());
                shad->set_vec3("lightColor", skyColor.get_rgb_vec3());
                shad->set_vec3("lightPos", lightPos);
                input.set_shader(shad);
                character.update_shader(shad); });
    if (lastSubdivision != subdivision)
    {
        lastSubdivision = subdivision;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(5);
    // enable gl wireframe mode
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    objBuffer.draw();
    // character.draw();

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::should_close()
{
    return glfwWindowShouldClose(window);
}