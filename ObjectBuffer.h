#pragma once
#include <vector>

#include "ITriangleObject.h"

class ObjectBuffer
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<ITriangleObject*> objects;
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;
    void generate_draw();

public:
    ObjectBuffer();
    ObjectBuffer(const ObjectBuffer&) = delete;
    ObjectBuffer(ObjectBuffer&&) = delete;
    ObjectBuffer &operator=(const ObjectBuffer&) = delete;
    ObjectBuffer &operator=(ObjectBuffer&&) = delete;

    void draw() const;
    void add_object(ITriangleObject* object);
    void destroy_object(ITriangleObject* object);
    std::vector<ITriangleObject*> get_objects() const;

    ~ObjectBuffer();
};
