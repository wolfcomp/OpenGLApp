#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "Cube.h"
#include "InputProcessing.h"
#include "Shader.h"
#include "Misc.h"
#include "ObjectBuffer.h"

constexpr int width = 800;
constexpr int height = 800;
float lastX = 400, lastY = 300;
bool firstMouse = true;

InputProcessing input;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_mouse_input(GLFWwindow* window, const double x_pos, const double y_pos)
{

    if (firstMouse)
    {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }

    float x_offset = x_pos - lastX;
    float y_offset = lastY - y_pos; // reversed: y ranges bottom to top
    lastX = x_pos;
    lastY = y_pos;

    input.process_mouse_movement(x_offset, y_offset);
}

int main()
{
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

    ObjectBuffer objBuffer;

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

    Shader shader("shader.vs", "shader.fs");
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        const double deltaTime = glfwGetTime() - lastTime;
        input.process_keyboard(window, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //enable gl wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader.use();
        input.set_shader(&shader);

        objBuffer.draw();

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        input.reset();
        lastTime = glfwGetTime();
    }

    glfwTerminate();

    return 0;
}
