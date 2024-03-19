#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
unsigned int TextureFromEmbed(const aiTexture *texture, const aiTextureType &type, bool has_alpha = false, bool gamma = false);

struct MaterialProperties
{
    struct MaterialGroup
    {
        aiShadingMode shading_mode; // $mat.shadingm
        float shininess_strength;   // $mat.shinpercent
        float shininess;            // $mat.shininess
        float roughness;            // $mat.roughnessFactor
        float transparency_factor;  // $mat.transparencyFactor
        float opacity;              // $mat.opacity
        float reflectivity;         // $mat.reflectivity
        float bump_scaling;         // $mat.bumpscaling
        float displacement_scaling; // $mat.displacementscaling
    };

    struct FileGroup
    {
        unsigned name_length;
        // if name contains ** then its an embedded texture to be loaded
        // if name contains * then its a file texture to be loaded
        std::string name; // file
        float uvtrafo;    // uvtrafo
        int uvwsrc;       // uvwsrc
    };

    struct ColorGroup
    {
        glm::vec4 diffuse;     // $clr.diffuse
        glm::vec4 ambient;     // $clr.ambient
        glm::vec4 specular;    // $clr.specular
        glm::vec4 emissive;    // $clr.emissive
        glm::vec4 transparent; // $clr.transparent
        glm::vec4 reflective;  // $clr.reflective
    };

    std::string name;
    MaterialGroup group;
    std::map<std::string, FileGroup> raw_groups; // $raw.<name>|
    std::map<unsigned, FileGroup> textures;      // $tex
    ColorGroup colors;
};

class Model
{
public:
    // model data
    vector<Texture> textures_loaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader &shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        meshes.reserve(scene->mNumMeshes);
        textures_loaded.reserve(scene->mNumTextures);

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces * 3);

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texture_coords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.texture_coords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName, const aiScene *scene)
    {
        vector<Texture> textures;
        MaterialProperties properties;
        vector<aiMaterialProperty> properties_list;
        properties_list.reserve(mat->mNumProperties);
        for (int i = 0; i < mat->mNumProperties; ++i)
        {
            const auto prop = mat->mProperties[i];
            properties_list.push_back(*prop);
            const auto key = std::string(prop->mKey.C_Str());
            const auto indexer = key[0];
            const auto loc = key.substr(1, 3);
            if (indexer == '?')
            {
                properties.name = ((aiString *)prop->mData)->C_Str();
            }
            else
            {
                if (loc == "mat")
                {
                    if (key == "$mat.shadingm")
                        properties.group.shading_mode = (aiShadingMode)(*(int *)prop->mData);
                    else if (key == "$mat.shinpercent")
                        properties.group.shininess_strength = *(float *)prop->mData;
                    else if (key == "$mat.shininess")
                        properties.group.shininess = *(float *)prop->mData;
                    else if (key == "$mat.roughnessFactor")
                        properties.group.roughness = *(float *)prop->mData;
                    else if (key == "$mat.transparencyFactor")
                        properties.group.transparency_factor = *(float *)prop->mData;
                    else if (key == "$mat.opacity")
                        properties.group.opacity = *(float *)prop->mData;
                    else if (key == "$mat.reflectivity")
                        properties.group.reflectivity = *(float *)prop->mData;
                    else if (key == "$mat.bumpscaling")
                        properties.group.bump_scaling = *(float *)prop->mData;
                    else if (key == "$mat.displacementscaling")
                        properties.group.displacement_scaling = *(float *)prop->mData;
                }
                else if (loc == "clr")
                {
                    const auto value = (aiColor4D *)prop->mData;
                    if (key == "$clr.diffuse")
                        properties.colors.diffuse = glm::vec4(value->r, value->g, value->b, 1);
                    else if (key == "$clr.ambient")
                        properties.colors.ambient = glm::vec4(value->r, value->g, value->b, 1);
                    else if (key == "$clr.specular")
                        properties.colors.specular = glm::vec4(value->r, value->g, value->b, 1);
                    else if (key == "$clr.emissive")
                        properties.colors.emissive = glm::vec4(value->r, value->g, value->b, 1);
                    else if (key == "$clr.transparent")
                        properties.colors.transparent = glm::vec4(value->r, value->g, value->b, 1);
                    else if (key == "$clr.reflective")
                        properties.colors.reflective = glm::vec4(value->r, value->g, value->b, 1);
                }
                else if (loc == "raw")
                {
                    const auto name_key = key.substr(5);
                    const auto name = name_key.substr(0, name_key.find("|"));
                    if (name == "Shininess")
                        continue;
                    if (name_key.ends_with("file"))
                    {
                        // can start with an integer
                        const auto name_index = *(int *)prop->mData;
                        bool valid = true;
                        if (name_index > scene->mNumTextures)
                            valid = false;
                        const auto byte_vec = std::vector<char>(prop->mData + (valid ? 4 : 0), prop->mData + prop->mDataLength);
                        properties.raw_groups[name].name = (valid ? to_string(name_index) : "") + "*" + std::string(byte_vec.begin(), byte_vec.end());
                        properties.raw_groups[name].name_length = prop->mDataLength;
                    }
                    else if (name_key.ends_with("uvtrafo"))
                        properties.raw_groups[name].uvtrafo = *(float *)prop->mData;
                    else if (name_key.ends_with("uvwsrc"))
                        properties.raw_groups[name].uvwsrc = *(int *)prop->mData;
                }
            }
        }

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            const auto alpha = string(str.C_Str()).find(".png") != string::npos;
            const auto gamma = type == aiTextureType_DIFFUSE;
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            { // if texture hasn't been loaded already, load it
                Texture texture;
                aiString texture_file;
                mat->Get(AI_MATKEY_SHADER_COMPUTE(type, i), texture_file);
                std::cout << "Texture: " << str.C_Str() << "\n";
                if (texture_file.data[0] == '*')
                {
                    const auto index = atoi(&texture_file.C_Str()[1]);
                    texture.id = TextureFromEmbed(scene->mTextures[index], type, alpha, gamma);
                }
                else
                    texture.id = TextureFromFile(str.C_Str(), this->directory, gamma);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
            }
        }
        return textures;
    }
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        GLenum internal_format;
        if (nrComponents == 1)
        {
            format = GL_RED;
            internal_format = GL_RED;
        }
        else if (nrComponents == 3)
        {
            format = GL_RGB;
            internal_format = gamma ? GL_SRGB : GL_RGB;
        }
        else if (nrComponents == 4)
        {
            format = GL_RGBA;
            internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

enum class TextureFormat
{
    RGBA8888,
    ARGB8888,
    RGBA5650,
    RGBA0010
};

inline TextureFormat get_texture_format(const std::string &format)
{
    if (format == "rgba8888")
        return TextureFormat::RGBA8888;
    else if (format == "argb8888")
        return TextureFormat::ARGB8888;
    else if (format == "rgba5650")
        return TextureFormat::RGBA5650;
    else if (format == "rgba0010")
        return TextureFormat::RGBA0010;
    else
        return TextureFormat::RGBA8888;
};

unsigned int TextureFromEmbed(const aiTexture *texture, const aiTextureType &type, bool has_alpha, bool gamma)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    const auto format_string = std::string(texture->achFormatHint);
    std::cout << "Loading texture with format: " << format_string << "\n";

    GLenum format;
    GLenum internal_format;

    switch (get_texture_format(format_string))
    {
    case TextureFormat::RGBA8888:
        format = GL_RGBA;
        internal_format = gamma ? GL_SRGB_ALPHA : GL_RGBA;
        break;
    case TextureFormat::ARGB8888:
        format = GL_BGRA;
        internal_format = gamma ? GL_SRGB_ALPHA : GL_BGRA;
        break;
    default:
        format = GL_RGB;
        internal_format = gamma ? GL_SRGB : GL_RGB;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, texture->mWidth, texture->mHeight, 0, format, GL_UNSIGNED_BYTE, texture->pcData);

    return textureID;
}
#endif