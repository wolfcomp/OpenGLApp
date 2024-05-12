#pragma once
#include <string>
#include <imgui.h>

class ImGuiWindow
{
    std::string name;
    bool open;
    float x, y, width, height;
    float opacity;
    ImGuiWindowFlags flags;

public:
    virtual ~ImGuiWindow() = default;
    ImGuiWindow(const std::string &name, bool open, float x, float y, float width, float height, float opacity)
        : name(name), open(open), x(x), y(y), width(width), height(height), opacity(opacity), flags(ImGuiWindowFlags_None) {}

    ImGuiWindow(const std::string &name, bool open)
        : name(name), open(open), x(0), y(0), width(0), height(0), opacity(1.0f), flags(ImGuiWindowFlags_None) {}

    ImGuiWindow(const std::string &name)
        : name(name), open(false), x(0), y(0), width(0), height(0), opacity(1.0f), flags(ImGuiWindowFlags_None) {}

    ImGuiWindow()
        : name(""), open(false), x(0), y(0), width(0), height(0), opacity(1.0f), flags(ImGuiWindowFlags_None) {}

    void set_position(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    void set_size(float width, float height)
    {
        this->width = width;
        this->height = height;
    }
    void set_opacity(float opacity) { this->opacity = opacity; }
    void set_flags(ImGuiWindowFlags flags) { this->flags = flags; }

    void show() { open = true; }
    void hide() { open = false; }
    void toggle() { open = !open; }

    bool is_open() const { return open; }
    std::string get_name() const { return name; }
    ImGuiWindowFlags get_flags() const { return flags; }

    virtual void render() = 0;
};