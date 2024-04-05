#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texture_coord;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 bitangent;
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];

    Vertex()
    {
        memset(m_BoneIDs, 0, sizeof(m_BoneIDs));
        memset(m_Weights, 0, sizeof(m_Weights));
        position = glm::vec3(0.0f);
        normal = glm::vec3(0.0f);
        texture_coord = glm::vec2(0.0f);
        tangent = glm::vec3(0.0f);
        bitangent = glm::vec3(0.0f);
    }

    Vertex(const glm::vec3 &pos)
    {
        position = pos;
        normal = glm::vec3(0.0f);
        texture_coord = glm::vec2(0.0f);
        tangent = glm::vec3(0.0f);
        bitangent = glm::vec3(0.0f);
    }

    Vertex(const glm::vec3 &pos, const glm::vec3 &norm)
    {
        position = pos;
        normal = norm;
        texture_coord = glm::vec2(0.0f);
        tangent = glm::vec3(0.0f);
        bitangent = glm::vec3(0.0f);
    }

    Vertex(const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &texCoord)
    {
        position = pos;
        normal = norm;
        texture_coord = texCoord;
        tangent = glm::vec3(0.0f);
        bitangent = glm::vec3(0.0f);
    }

    Vertex(const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &texCoord, const glm::vec3 &tangent, const glm::vec3 &bitangent)
    {
        position = pos;
        normal = norm;
        texture_coord = texCoord;
        this->tangent = tangent;
        this->bitangent = bitangent;
    }
};

struct Texture
{
    unsigned int id;
    string type;
    string path;
};

class Mesh
{
public:
    // mesh Data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader &shader)
    {
        // bind appropriate textures
        unsigned int diffuseNr = 0;
        unsigned int specularNr = 0;
        unsigned int normalNr = 0;
        unsigned int heightNr = 0;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if (name == "texture_specular")
                number = std::to_string(specularNr++); // transfer unsigned int to string
            else if (name == "texture_normal")
                number = std::to_string(normalNr++); // transfer unsigned int to string
            else if (name == "texture_height")
                number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    // render data
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coord));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};
#endif