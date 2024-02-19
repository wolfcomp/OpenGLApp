#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "src/InputProcessing.h"
#include "src/Misc.h"
#include "src/ObjectBuffer.h"
#include "src/ShaderStore.h"
#include "src/primitives/Capsule.h"
#include "src/primitives/Cube.h"
#include "src/primitives/IcoSphere.h"
#include "src/primitives/Sphere.h"

constexpr int width = 1600;
constexpr int height = 900;
float lastX = 400, lastY = 300;
bool firstMouse = true;
int subdivision = 0;
bool dirty = true;
bool wireframe = false;
glm::vec3 staticCamPos = glm::vec3(0.f, 0.f, 3.f);
float staticCamYaw = 0.f;
float staticCamPitch = 0.f;

InputProcessing input;
ObjectBuffer objBuffer;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    input.change_aspect(width, height);
}

void increase_subdivision()
{
    if (subdivision < 5)
    {
        subdivision++;
        dirty = true;
    }
}

void decrease_subdivision()
{
    if (subdivision > 0)
    {
        subdivision--;
        dirty = true;
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

    const float x_offset = x_pos - lastX;
    const float y_offset = lastY - y_pos; // reversed: y ranges bottom to top
    lastX = x_pos;
    lastY = y_pos;

    input.process_mouse_movement(x_offset, y_offset);
}

int main()
{
    input.change_aspect(width, height);
    input.set_camera_position(glm::vec3(-4.f, 3.f, 6.5f));
    input.set_camera_rotation(-50.f, -17.f);
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

    input.attach_keyboard_listener(GLFW_KEY_UP, increase_subdivision, false);
    input.attach_keyboard_listener(GLFW_KEY_DOWN, decrease_subdivision, false);
    input.attach_keyboard_listener(GLFW_KEY_F, []() { wireframe = !wireframe; }, false);

    objBuffer.init_buffers();

    auto cube = Cube();
    auto cube2 = Cube();
    auto cube3 = Cube();

    cube2.set_euler_rotation(glm::vec3(.4f, .2f, .6f));
    cube3.set_euler_rotation(glm::vec3(.6f, -.8f, .1f));
    cube2.set_scale(glm::vec3(1.5f));
    cube3.set_scale(glm::vec3(0.5f));
    cube2.set_position(glm::vec3(3.f));
    cube3.set_position(glm::vec3(-2.f));
    cube2.set_color(hsl(120, 1, .5f));
    cube3.set_color(hsl(240, 1, .5f));

    objBuffer.add_object(&cube);
    objBuffer.add_object(&cube2);
    objBuffer.add_object(&cube3);

    auto ico_sphere = IcoSphere();

    ico_sphere.set_radius(0.3f);
    ico_sphere.set_color(hsl(0, 0.5f, .5f));
    ico_sphere.set_position(glm::vec3(3.f, -2.f, -1.f));

    objBuffer.add_object(&ico_sphere);

    Shader shader("shader.vs", "shader.fs");
    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        const double delta_time = glfwGetTime() - last_time;
        input.process_keyboard(window, delta_time);
        if (dirty)
        {
            ico_sphere.set_subdivision(subdivision);
            objBuffer.update_buffers();
            dirty = false;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //enable gl wireframe mode
        if(wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader.use();
        input.set_shader(&shader);

        objBuffer.draw();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        input.reset();
        last_time = glfwGetTime();
    }

    glfwTerminate();

    return 0;
}
