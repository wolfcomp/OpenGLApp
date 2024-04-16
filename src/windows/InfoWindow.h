#pragma once
#include "ImGuiWindow.h"
#include <chrono>
#include "../Camera.h"
#include <imgui.h>

class InfoWindow : public ImGuiWindow
{
    std::chrono::time_point<std::chrono::high_resolution_clock> last_draw;
    Camera *camera;

public:
    InfoWindow(Camera *camera)
        : ImGuiWindow("Info", true)
    {
        set_flags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
        this->camera = camera;
        last_draw = std::chrono::high_resolution_clock::now();
    }

    void render() override
    {
        ImGui::Text("Camera position: (%.2f, %.2f, %.2f)", camera->get_pos().x, camera->get_pos().y, camera->get_pos().z);
        ImGui::Text("Camera rotation: (%.2f, %.2f, %.2f)", camera->get_rotation().x, camera->get_rotation().y, camera->get_rotation().z);
        ImGui::Separator();
        ImGui::Text("Frame rate: %.2f", 1.0 / std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - last_draw).count());
        last_draw = std::chrono::high_resolution_clock::now();
    }
};