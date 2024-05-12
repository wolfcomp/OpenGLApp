#include "src/Window.h"

#include <iostream>

#include "src/ShaderStore.h"
#include <filesystem>

Window *window;

int main()
{
    auto path = std::filesystem::current_path();
    // check if path ends with Debug if not append Debug and set as current path
    if (path.string().find("Debug") == std::string::npos)
    {
        path = path / "Debug";
        std::filesystem::current_path(path);
    }

    window = new Window();

    auto winRet = window->init();
    if (winRet != 0)
    {
        return winRet;
    }
    ShaderStore::add_shader("default", "shaders/default.vert", "shaders/default.frag");
    ShaderStore::add_shader("uvVisual", "shaders/default.vert", "shaders/texCoordVisual.frag");
    ShaderStore::add_shader("normalVisual", "shaders/default.vert", "shaders/normalVisual.frag");
    ShaderStore::add_shader("noLight", "shaders/default.vert", "shaders/noLight.frag");
    ShaderStore::add_shader("shadow", "shaders/shadowMap.vert", "shaders/shadowMap.frag");

    window->create_objects();

    while (!window->should_close())
    {
        window->update();
    }

    delete window;

    return 0;
}
