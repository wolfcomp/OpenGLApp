#include "ObjectBuffer.h"

#include "glad/glad.h"

ObjectBuffer::~ObjectBuffer()
{
    vertices.clear();
    indices.clear();
    objects.clear();
}

void ObjectBuffer::init_buffers()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glEnableVertexAttribArray(0);
    // ReSharper disable CppCStyleCast
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coord));
    // ReSharper restore CppCStyleCast
    glBindVertexArray(0);
}


void ObjectBuffer::draw() const
{
    glBindVertexArray(vao);
    for (const auto& object : objects)
    {
        object->draw();
    }
}


void ObjectBuffer::add_object(ITriangleObject* object)
{
    objects.push_back(object);
}

void ObjectBuffer::destroy_object(ITriangleObject* object)
{
    const auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end())
    {
        objects.erase(it);
    }
}

std::vector<ITriangleObject*> ObjectBuffer::get_objects() const
{
    return objects;
}

