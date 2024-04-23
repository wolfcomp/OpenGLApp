#pragma once

#include <string>
#include <vector>
#include "objects/Mesh.h"

class ModelImport
{
public:
    static Mesh *load_model(const std::string &path);
};