#pragma once
#include <glm/vec3.hpp>
#include "HSL.h"
#include "Shader.h"
#include "Shadow.h"
#include <vector>

struct Light
{
    hsl ambient;
    hsl diffuse;
    glm::vec3 specular;

    virtual void set_shader(const Shader *shader)
    {
        shader->set_vec3(get_name() + ".light.ambient", ambient.get_rgb_vec3());
        shader->set_vec3(get_name() + ".light.diffuse", diffuse.get_rgb_vec3());
        shader->set_vec3(get_name() + ".light.specular", specular);
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

    glm::mat4 get_light_space_matrix(glm::vec3 position)
    {
        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(-direction * 10.0f, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        return lightProjection * lightView;
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

class LightManager
{
    std::vector<Light *> lights;

public:
    void add_light(Light *light)
    {
        lights.push_back(light);
    }

    void set_shader(const Shader *shader)
    {
        for (auto light : lights)
        {
            light->set_shader(shader);
        }
    }

    void bind_depth_maps(const Shader *shader)
    {
        for (auto light : lights)
        {
            auto spotLight = dynamic_cast<SpotLight *>(light);
            if (spotLight)
            {
                spotLight->bind_depth_map(shader);
            }
        }
    }

    void cleanup()
    {
        for (auto light : lights)
        {
            delete light;
        }
        lights.clear();
    }
};