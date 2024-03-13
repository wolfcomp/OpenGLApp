#pragma once
#include "../interfaces/IObject.h"

class Arrow : public IObject
{
public:
    Arrow()
    {
        vertices = {
            Vertex{glm::vec3(0)},
            Vertex{glm::vec3(1, 0, 0)},
            Vertex{glm::vec3(0.5f, 0.45f, 0)},
            Vertex{glm::vec3(0.5f, -0.45f, 0)},
            Vertex{glm::vec3(0.5f, 0, 0.45f)},
            Vertex{glm::vec3(0.5f, 0, -0.45f)}};
        indices = {
            0, 1, // line
            1, 2, // top
            1, 3, // bottom
            1, 4, // left
            1, 5, // right
        };
        draw_mode = GL_LINES;
    }
};