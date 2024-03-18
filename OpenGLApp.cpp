#include "src/Window.h"

#include <iostream>

#include "src/ShaderStore.h"
#include "src/TimeManager.h"
#include <filesystem>

Window window;

int main()
{
    std::cout << std::filesystem::current_path() << std::endl;

    auto winRet = window.init();
    if (winRet != 0)
    {
        return winRet;
    }
    ShaderStore::add_shader("default", "default.vs", "default.fs");
    ShaderStore::add_shader("uvVisual", "default.vs", "texCoordVisual.fs");
    ShaderStore::add_shader("noLight", "default.vs", "noLight.fs");
    ShaderStore::add_shader("shadowMap", "shadowMap.vs", "shadowMap.fs");

    window.create_objects();

    TimeManager::set_last_frame(glfwGetTime());

    while (!window.should_close())
    {
        window.update();
    }

    glfwTerminate();

    return 0;
}
