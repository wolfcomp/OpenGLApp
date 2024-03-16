#pragma once
#include <vector>

#include "../Shader.h"
#include "../Vertex.h"
#include "glad/glad.h"
#include "../HSL.h"
#include "glm/gtc/type_ptr.hpp"
#include "../Material.h"
#include "IRender.h"

class IObject : public IRenderOld
{
    std::vector<IObject *> childern;

public:
    virtual ~IObject() = default;
    IObject() = default;
    IObject(const IObject &) = default;
    IObject &operator=(const IObject &) = default;
    IObject(IObject &&) = default;
    IObject &operator=(IObject &&) = default;
    ICollisionOld *collision = nullptr;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool should_draw = true;

    void draw(Shader *override = nullptr) override
    {
        if (!this->should_draw || shader == nullptr || vertices.empty() || indices.empty())
            return;
        IRenderOld::draw(override);
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

    virtual std::vector<ICollisionOld *> get_collisions()
    {
        return {collision};
    }

    void add_child(IObject *child)
    {
        childern.push_back(child);
    }

    void remove_child(IObject *child)
    {
        childern.erase(std::remove(childern.begin(), childern.end(), child), childern.end());
    }

    void remove_all_children()
    {
        childern.clear();
    }

    void draw_children()
    {
        for (auto &child : childern)
        {
            child->draw();
        }
    }
};
