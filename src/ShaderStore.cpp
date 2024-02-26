#include "ShaderStore.h"

Shader* ShaderStore::add_shader(const char* vertex_path, const char* fragment_path)
{
    const auto shader = new Shader(vertex_path, fragment_path);
    shaders[shaders.size()] = shader;
    return shader;
}

Shader* ShaderStore::get_shader(const unsigned id)
{
    return shaders[id];
}

void ShaderStore::remove_all_shaders()
{
    for (const auto& shader : shaders)
    {
        delete shader.second;
    }
    shaders.clear();
}

void ShaderStore::remove_shader(const unsigned id)
{
    delete shaders[id];
    shaders.erase(id);
}

void ShaderStore::set_shader_params(void func(const Shader*)) const
{
    for (const auto& shaderPair : shaders)
    {
        const auto shader = shaderPair.second;
        func(shader);
    }
}
