#pragma once
#include "../ShaderStore.h"
#include "../interfaces/IObject.h"

class PointRender : public IObject
{
public:
    PointRender() : IObject()
    {
        draw_mode = GL_POINTS;
        shader = ShaderStore::get_shader("default");
    }

    void add_points(const std::vector<glm::vec2>& points)
    {
        Vertex vertex;
        vertex.color = glm::vec3(.5f, .5f, .5f);
        for (const auto& point : points)
        {
            vertex.position = glm::vec3(point.x, 0, point.y);
            vertices.push_back(vertex);
            indices.push_back(vertices.size() - 1);
        }
    }

    void clear_points()
    {
        vertices.clear();
        indices.clear();
    }
};
