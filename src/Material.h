#pragma once
#include <string>

class Shader;

struct Material
{
    unsigned int diffuseTexture = 0;
    unsigned int specularTexture = 0;
    unsigned int normalTexture = 0;
    float shininess = 32.0f;
    Shader *shader;
    Shader *shadow_shader;

    void load_texture(const std::string block, const std::string path);
    void set_shininess(float shininess);
    void use();
};