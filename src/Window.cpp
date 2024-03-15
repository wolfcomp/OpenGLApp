#include "Window.h"
#include <iostream>
#include "TimeManager.h"
#include "ObjectBuffer.h"
#include "objects/Character.h"
#include "debug/Arrow.h"
#include "ShaderStore.h"
#include "primitives/Cube.h"
#include "InputProcessing.h"
#include "Light.h"
#include "Math.h"

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
hsl pointColor = hsl(0, 0, 0);
hsl ambientColor = hsl(0, 0, .05f);
glm::vec3 lightPos = glm::vec3(0, 0, 4);
DirectionalLight dirLight;
PointLight light0;
PointLight light1;
PointLight light2;
PointLight light3;
SpotLight spotLight;

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

Material *material = nullptr;
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

    window = glfwCreateWindow(width, height, "OpenGLApp", nullptr, nullptr);
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
    return 0;
}

void Window::create_objects()
{
    objBuffer.init_buffers();

    material = new Material();

    material->load_texture("container", "container.png");

    for (int i = 0; i < 10; i++)
    {
        const auto cube = new Cube();

        cube->set_position(cubePositions[i]);
        const auto angle = 20.0f * i;
        cube->set_euler_rotation(glm::vec3(1.0f, 0.3f, 0.5f) * angle);
        cube->set_scale(glm::vec3(.5f, .5f, .5f));
        cube->set_albedo(hsl(0, .6f, .5f));
        cube->set_shader(ShaderStore::get_shader("default"));
        cube->material = material;

        objBuffer.add_object(cube);
    }

    const auto cube2 = new Cube();

    cube2->set_position(pointLightPositions[0]);
    cube2->set_scale(glm::vec3(.1f, .1f, .1f));
    cube2->set_albedo(pointColor);
    cube2->set_shader(ShaderStore::get_shader("noLight"));

    objBuffer.add_object(cube2);

    const auto arrow = new Arrow();
    arrow->set_position(pointLightPositions[0]);
    arrow->set_rotation(glm::vec3(0, M_PI / 2, 0));
    arrow->set_albedo(hsl(0, .5, .5));
    arrow->set_shader(ShaderStore::get_shader("noLight"));

    objBuffer.add_object(arrow);

    light0.index = 0;
    light0.position = pointLightPositions[0];
    light0.diffuse = pointColor;
    light0.ambient = ambientColor;
    light0.specular = glm::vec3(0);
    light0.constant = 1.0f;
    light0.linear = 0.09f;
    light0.quadratic = 0.032f;

    light1 = light2 = light3 = light0;
    light1.index = 1;
    light2.index = 2;
    light3.index = 3;
    light1.position = pointLightPositions[1];
    light2.position = pointLightPositions[2];
    light3.position = pointLightPositions[3];

    dirLight.ambient = hsl(0, 0, .05f);
    dirLight.diffuse = hsl(0, 0, 0);
    dirLight.specular = glm::vec3(0);
    dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    spotLight.position = staticCameraPos;
    spotLight.direction = glm::vec3(0, 0, 0);
    spotLight.constant = 1.0f;
    spotLight.linear = 0.09f;
    spotLight.quadratic = 0.032f;
    spotLight.ambient = hsl(0, 0, .05f);
    spotLight.diffuse = hsl(0, 0, 0);
    spotLight.specular = glm::vec3(0);

    character.set_position(pointLightPositions[0] - glm::vec3(2, 0, 0));
    character.set_shader(ShaderStore::get_shader("noLight"));

    ShaderStore::set_shader_params(
        [](const Shader *shad)
        {
            light0.set_shader(shad);
            light1.set_shader(shad);
            light2.set_shader(shad);
            light3.set_shader(shad);
            dirLight.set_shader(shad);
            spotLight.set_shader(shad);
            input.set_shader(shad);
            character.update_shader(shad);
        });
}

void Window::update() const
{
    TimeManager::set_current_frame(glfwGetTime());
    TimeManager::set_delta_time((TimeManager::get_current_frame() - TimeManager::get_last_frame()) / 1000);
    TimeManager::set_last_frame(TimeManager::get_current_frame());
    input.process_keyboard(window, TimeManager::get_delta_time());
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

bool Window::should_close() const
{
    return glfwWindowShouldClose(window);
}
