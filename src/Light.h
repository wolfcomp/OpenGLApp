#pragma once
#include <glm/vec3.hpp>
#include "HSL.h"
#include "Shader.h"
#include "Shadow.h"

struct Light
{
    hsl ambient;
    hsl diffuse;
    glm::vec3 specular;

    virtual void set_shader(const Shader *shader)
    {
        shader->set_vec3(get_name() + ".ambient", ambient.get_rgb_vec3());
        shader->set_vec3(get_name() + ".diffuse", diffuse.get_rgb_vec3());
        shader->set_vec3(get_name() + ".specular", specular);
    }

    virtual std::string get_name()
    {
        return "light";
    }
};

struct DirectionalLight : public Light
{
    glm::vec3 direction;

    void set_shader(const Shader *shader) override
    {
        Light::set_shader(shader);
        shader->set_vec3(get_name() + ".direction", direction);
    }

    std::string get_name() override
    {
        return "dirLight";
    }
};

struct PointLight : public Light
{
    int index;
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    void set_shader(const Shader *shader) override
    {
        Light::set_shader(shader);
        shader->set_vec3(get_name() + ".position", position);
        shader->set_float(get_name() + ".constant", constant);
        shader->set_float(get_name() + ".linear", linear);
        shader->set_float(get_name() + ".quadratic", quadratic);
    }

    std::string get_name() override
    {
        return "pointLights[" + std::to_string(index) + "]";
    }
};

struct SpotLight : public PointLight
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    ShadowProcessor *shadowProcessor;

    SpotLight()
    {
        shadowProcessor = new ShadowProcessor();
        shadowProcessor->init(16, get_name() + ".shadowMap");
    }

    void set_shader(const Shader *shader) override
    {
        PointLight::set_shader(shader);
        shader->set_vec3(get_name() + ".direction", direction);
        shader->set_float(get_name() + ".cutOff", cutOff);
        shader->set_float(get_name() + ".outerCutOff", outerCutOff);
    }

    void bind_depth_map(const Shader *shader)
    {
        shadowProcessor->bind_depth_map(shader);
    }

    std::string get_name() override
    {
        return "spotLight";
    }
};