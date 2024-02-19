#pragma once
#include <map>

#include "InputProcessing.h"
#include "Shader.h"

class ShaderStore
{
    std::map<unsigned, Shader*> shaders;

public:
    Shader* get_shader(const unsigned id);
    Shader* add_shader(const char* vertex_path, const char* fragment_path);
    void remove_shader(const unsigned id);
    void remove_all_shaders();
    void set_shader_params(InputProcessing* input) const;
};

