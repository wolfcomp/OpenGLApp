#pragma once
#include <vector>

#include "../Shader.h"
#include "../Vertex.h"
#include "../collision/ICollision.h"
#include "glad/glad.h"

class IObject
{
public:
    virtual ~IObject() = default;
    IObject() = default;
    IObject(const IObject&) = default;
    IObject &operator=(const IObject&) = default;
    IObject(IObject&&) = default;
    IObject &operator=(IObject&&) = default;
    Shader* shader;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool should_draw = true;
    ICollision* collision;
    GLenum draw_mode = GL_POINTS;
    void draw()
    {
        if (!this->should_draw)
            return;
        if(shader != nullptr)
            shader->use();
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
    template <typename T, typename ... V>
    static void push_vector(std::vector<T>& vector, V ... args)
    {
        push_vector(vector, std::forward<V>(args)...);
    }
    template <typename T>
    static void push_vector(std::vector<T>& vector, T arg)
    {
        vector.push_back(arg);
    }
    template <typename T, typename ... V>
    static void push_vector(std::vector<T>& vector, T arg, V ... args)
    {
        vector.push_back(arg);
        push_vector(vector, args...);
    }
};