#pragma once
#include <vector>

#include "../Shader.h"
#include "../Vertex.h"
#include "../collision/ICollision.h"
#include "glad/glad.h"
#include "../HSL.h"
#include "glm/gtc/type_ptr.hpp"

class IObject
{
public:
    virtual ~IObject() = default;
    IObject() = default;
    IObject(const IObject&) = default;
    IObject& operator=(const IObject&) = default;
    IObject(IObject&&) = default;
    IObject& operator=(IObject&&) = default;
    Shader* shader;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale = glm::vec3(1.0f);
    hsl color;
    float shininess = 32.0f;
    float specular = .5f;
    float metallic = .5f;
    glm::mat4 model = glm::mat4(1.0f);
    bool should_draw = true;
    ICollision* collision;
    GLenum draw_mode = GL_POINTS;

    void draw()
    {
        if (!this->should_draw || shader == nullptr)
            return;
        shader->use();
        shader->set_vec3("albedo", color.get_rgb_vec3());
        shader->set_float("specular", specular);
        shader->set_float("metallic", metallic);
        shader->set_float("shininess", shininess);
        shader->set_mat4("model", value_ptr(model));
        pre_draw();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glDrawElements(draw_mode, indices.size(), GL_UNSIGNED_INT, nullptr);

        post_draw();
    }

    virtual void pre_draw()
    {
    }

    virtual void post_draw()
    {
    }

    void set_shader(Shader* shader)
    {
        this->shader = shader;
    }

    void compute_model_matrix()
    {
        model = glm::mat4(1.0f);
        model *= mat4_cast(rotation);
        model = translate(model, position);
        model = glm::scale(model, scale);
    }

    template <typename T, typename... V>
    static void push_vector(std::vector<T>& vector, V... args)
    {
        push_vector(vector, std::forward<V>(args)...);
    }

    template <typename T>
    static void push_vector(std::vector<T>& vector, T arg)
    {
        vector.push_back(arg);
    }

    template <typename T, typename... V>
    static void push_vector(std::vector<T>& vector, T arg, V... args)
    {
        vector.push_back(arg);
        push_vector(vector, args...);
    }

    virtual std::vector<ICollision*> get_collisions()
    {
        return { collision };
    }
};
