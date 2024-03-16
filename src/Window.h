#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Window
{
    GLFWwindow *window;

    int init();
    static void create_objects();
    void update() const;
    void render() const;
    bool should_close() const;
};