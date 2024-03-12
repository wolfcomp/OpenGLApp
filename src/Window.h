#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window
{
    GLFWwindow *window;

    int init();
    static void create_objects();
    void update() const;
    bool should_close() const;
};