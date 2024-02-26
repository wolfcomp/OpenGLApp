#pragma once
#include <vector>

#include "../Shader.h"
#include "../Vertex.h"
#include "../collision/ICollision.h"
#include "glad/glad.h"

class ITriangleObject
{
public:
    virtual ~ITriangleObject() = default;
    ITriangleObject() = default;
    ITriangleObject(const ITriangleObject&) = default;
    ITriangleObject &operator=(const ITriangleObject&) = default;
    ITriangleObject(ITriangleObject&&) = default;
    ITriangleObject &operator=(ITriangleObject&&) = default;
    Shader* shader;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool should_draw = true;
    ICollision* collision;
    void draw()
    {
        if (!this->should_draw)
            return;
        if(shader != nullptr)
            shader->use();
        pre_draw();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        post_draw();
    }
    virtual void pre_draw()
    {
        
    }
    virtual void post_draw()
    {
        
    }
};
