#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>

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
        for (auto window : windows)
        {
            if (window->open)
            {
                window->render();
            }
        }
    }
}