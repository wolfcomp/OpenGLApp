#include "ModelImport.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

Mesh *process_node(aiNode *node, const aiScene *scene)
{
    auto mesh = new Mesh();

    if (node->mNumMeshes == 0 && node->mNumChildren == 1)
    {
        return process_node(node->mChildren[0], scene);
    }
    else if (node->mNumMeshes == 0 && node->mNumChildren > 1)
    {
        goto processChildren;
    }
    else if (node->mNumMeshes == 0 && node->mNumChildren == 0)
    {
        return nullptr;
    }

    {
        auto mesh_index = node->mMeshes[0];
        auto mesh_data = scene->mMeshes[mesh_index];

        for (auto i = 0; i < mesh_data->mNumVertices; ++i)
        {
            Vertex vertex;
            vertex.position = glm::vec3(mesh_data->mVertices[i].x, mesh_data->mVertices[i].y, mesh_data->mVertices[i].z);
            vertex.normal = glm::vec3(mesh_data->mNormals[i].x, mesh_data->mNormals[i].y, mesh_data->mNormals[i].z);

            if (mesh_data->mTextureCoords[0])
            {
                vertex.tex_coords = glm::vec2(mesh_data->mTextureCoords[0][i].x, mesh_data->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.tex_coords = glm::vec2(0.0f, 0.0f);
            }

            mesh->vertices.push_back(vertex);
        }

        for (auto i = 0; i < mesh_data->mNumFaces; ++i)
        {
            auto face = mesh_data->mFaces[i];
            for (auto j = 0; j < face.mNumIndices; ++j)
            {
                mesh->indices.push_back(face.mIndices[j]);
            }
        }
    }

processChildren:
    for (auto i = 0; i < node->mNumChildren; ++i)
    {
        auto child = node->mChildren[i];
        mesh->add_child(process_node(child, scene));
    }

    return mesh;
}

Mesh *ModelImport::load_model(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenNormals | aiProcess_OptimizeMeshes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    return process_node(scene->mRootNode, scene);
};