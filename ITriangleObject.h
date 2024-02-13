#pragma once
#include <vector>

#include "Vertex.h"

class ITriangleObject
{
public:
    virtual ~ITriangleObject() = default;
    ITriangleObject() = default;
    ITriangleObject(const ITriangleObject&) = default;
    ITriangleObject &operator=(const ITriangleObject&) = default;
    ITriangleObject(ITriangleObject&&) = default;
    ITriangleObject &operator=(ITriangleObject&&) = default;
    virtual std::vector<Vertex> get_vertices() const
    {
        return {};
    }
    virtual std::vector<unsigned int> get_indices() const
    {
        return {};
    }
};
