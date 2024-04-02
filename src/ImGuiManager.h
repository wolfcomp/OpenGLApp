#pragma once
#include "windows/ImGuiWindow.h"
#include <vector>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdio.h>

class ImGuiManager
{
    std::vector<ImGuiWindow *> windows;

public:
    ImGuiManager()
    {
        windows = std::vector<ImGuiWindow *>();
    }

    void add_window(ImGuiWindow *window)
    {
        windows.push_back(window);
    }

    void render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        for (auto window : windows)
        {
            if (window->is_open())
            {
                window->render();
            }
        }

        ImGui::Render();
    }

    void render_draw_data()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};