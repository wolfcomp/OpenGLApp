#pragma once

struct GLFWwindow;

struct Window
{
    GLFWwindow *window;

    int init();
    static void create_objects();
    void update() const;
    void render() const;
    bool should_close() const;
};