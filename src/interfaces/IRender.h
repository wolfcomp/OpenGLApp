#pragma once
#include <vector>

#include "../Shader.h"
#include "../Vertex.h"
#include "../collision/ICollision.h"
#include "glad/glad.h"
#include "../HSL.h"
#include "glm/gtc/type_ptr.hpp"
#include "../Material.h"

class IRender
{
public:
    virtual ~IRender() = default;
    IRender() = default;
    IRender(const IRender &) = default;
    IRender &operator=(const IRender &) = default;
    IRender(IRender &&) = default;
    IRender &operator=(IRender &&) = default;
    glm::quat rotation = glm::quat(1, 0, 0, 0);
    glm::vec3 position;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    GLenum draw_mode = GL_POINTS;
    Shader *shader = nullptr;
    Material *material = nullptr;
    hsl albedo = hsl(0, 0, 0);

    virtual void draw()
    {
        shader->use();
        if (material != nullptr)
            material->set_shader(shader);
        shader->set_vec3("albedo", albedo.get_rgb_vec3());
        shader->set_mat4("model", value_ptr(model));
    }

    virtual void pre_draw()
    {
    }

    virtual void post_draw()
    {
    }

    virtual void set_shader(Shader *shader)
    {
        this->shader = shader;
    }

    void compute_model_matrix()
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = model * glm::mat4_cast(rotation);
        model = glm::scale(model, scale);
    }

    void set_position(glm::vec3 position)
    {
        this->position = position;
        compute_model_matrix();
    }

    void set_scale(glm::vec3 scale)
    {
        this->scale = scale;
        compute_model_matrix();
    }

    void set_rotation(glm::quat quaternion)
    {
        this->rotation = quaternion;
        compute_model_matrix();
    }

    void set_euler_rotation(glm::vec3 angle)
    {
        this->rotation = glm::quat(angle);
        compute_model_matrix();
    }

    void set_albedo(hsl color)
    {
        albedo = color;
    }
};
