#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window
{
    GLFWwindow *window;

    int init();
    void create_objects();
    void update();
    bool should_close();
};