#pragma once

#include "../Mesh.h"

class Empty : public Mesh
{
public:
    Empty() : Mesh()
    {
        should_draw = false;
    }
};