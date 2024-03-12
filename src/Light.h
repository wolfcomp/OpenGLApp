#pragma once
#include <glm/vec3.hpp>
#include "HSL.h"
#include "Shader.h"

enum class LightType
{
    DIRECTIONAL,
    POINT,
    SPOT
};

struct Light
{
    LightType lightType;
    glm::vec3 position;
    glm::vec3 direction;

    hsl color;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    void set_shader(const Shader *shader)
    {
        shader->set_int("light.type", static_cast<int>(lightType));
        shader->set_vec3("light.position", position);
        shader->set_vec3("light.diffuse", color.get_rgb_vec3());
        shader->set_vec3("light.specular", glm::vec3(1.0f));
        shader->set_float("light.constant", constant);
        shader->set_float("light.linear", linear);
        shader->set_float("light.quadratic", quadratic);
        shader->set_float("light.cutOff", cutOff);
        shader->set_float("light.outerCutOff", outerCutOff);
    }
};