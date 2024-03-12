#include "src/Window.h"

#include <iostream>

#include "src/ShaderStore.h"
#include "src/TimeManager.h"

Window window;

int main()
{
    auto winRet = window.init();
    if (winRet != 0)
    {
        return winRet;
    }
    ShaderStore::add_shader("default", "default.vs", "default.fs");
    ShaderStore::add_shader("uvVisual", "default.vs", "texCoordVisual.fs");
    ShaderStore::add_shader("noLight", "default.vs", "noLight.fs");
    ShaderStore::add_shader("lightSource", "default.vs", "lightSource.fs");

    TimeManager::set_last_frame(glfwGetTime());

    while (!window.should_close())
    {
        window.update();
    }

    glfwTerminate();

    return 0;
}
