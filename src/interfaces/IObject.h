#pragma once
#include <vector>

#include "../Shader.h"
#include "../Vertex.h"
#include "../collision/ICollision.h"
#include "glad/glad.h"
#include "../HSL.h"
#include "glm/gtc/type_ptr.hpp"
#include "../Material.h"
#include "IRender.h"

class IObject : public IRender
{
public:
    virtual ~IObject() = default;
    IObject() = default;
    IObject(const IObject &) = default;
    IObject &operator=(const IObject &) = default;
    IObject(IObject &&) = default;
    IObject &operator=(IObject &&) = default;
    ICollision *collision = nullptr;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool should_draw = true;

    void draw() override
    {
        if (!this->should_draw || shader == nullptr || material == nullptr)
            return;
        IRender::draw();
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

    template <typename T, typename... V>
    static void push_vector(std::vector<T> &vector, V... args)
    {
        push_vector(vector, std::forward<V>(args)...);
    }

    template <typename T>
    static void push_vector(std::vector<T> &vector, T arg)
    {
        vector.push_back(arg);
    }

    template <typename T, typename... V>
    static void push_vector(std::vector<T> &vector, T arg, V... args)
    {
        vector.push_back(arg);
        push_vector(vector, args...);
    }

    virtual std::vector<ICollision *> get_collisions()
    {
        return {collision};
    }
};
