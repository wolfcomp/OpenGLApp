#pragma once

#include <glad/glad.h>

#include <windows.h>
#include <fstream>
#include <vector>
#include "Math.h"
#include "Vertex.h"
#include "HSL.h"

class file_handle
{
    FILE* file;
public:
    file_handle(const char* path, const char* mode)
    {
        fopen_s(&file, path, mode);
    }
    void printf(_Printf_format_string_ char const* const format, ...) const
    {
        va_list args;
        va_start(args, format);
        vfprintf(file, format, args);
        va_end(args);
    }
    ~file_handle()
    {
        fclose(file);
    }
};

class func
{
    std::vector<Vertex> vertices;
    std::vector<Vertex> finalVertecies;
    std::vector<unsigned int> indices;

    float fX(float x)
    {
        return pow(x, 4) - 2 * pow(x, 3) + pow(x, 2);
    }
    float dfX(float x)
    {
        return 4 * pow(x, 3) - 6 * pow(x, 2) + 2 * x;
    }
    std::vector<Vertex> getVertices(float min, float max, float step, bool useZ)
    {
        std::vector<Vertex> vertices;
        auto diff = (max + min) / 2;
        while (min < max)
        {
            auto v = Vertex();
            v.position.x = min - diff;
            v.position.y = fX(min);
            if (useZ)
                v.position.z = dfX(min);
            min += step;
            vertices.push_back(v);
        }
        auto v = Vertex();
        v.position.x = min - diff;
        v.position.y = fX(min);
        if (useZ)
            v.position.z = dfX(min);
        vertices.push_back(v);
        return vertices;
    }
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;
public:
    func(bool useZ = false)
    {
        vertices = getVertices(-.3f, 1.3f, 0.1f, useZ);
        for (int i = 0; i < vertices.size() - 1; i++)
        {
            auto vertex = vertices[i];
            auto nextVertex = vertices[i + 1];
            if (nextVertex.position.y < vertex.position.y)
                vertex.color.r = 1;
            else
                vertex.color.g = 1;
            vertices[i] = vertex;
        }
        FILE* outpFile;
        fopen_s(&outpFile, "f_outp.txt", "w");
        fprintf(outpFile, "%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.position.x, vertex.position.y, vertex.position.z, vertex.color.r, vertex.color.g, vertex.color.b, vertex.texture_coord.x, vertex.texture_coord.y);
        }
        fclose(outpFile);
    }

    void initDraw()
    {
        for (int i = 0; i < vertices.size() - 1; i++)
        {
            auto vertex = vertices[i];
            auto nextVertex = vertices[i + 1];
            auto betweenVertex = Vertex();
            finalVertecies.push_back(vertex);
            indices.push_back(finalVertecies.size() - 1);
            betweenVertex.position.x = nextVertex.position.x;
            betweenVertex.position.y = nextVertex.position.y;
            betweenVertex.position.z = vertex.position.z;
            betweenVertex.color.r = vertex.color.r;
            betweenVertex.color.g = vertex.color.g;
            betweenVertex.color.b = vertex.color.b;
            finalVertecies.push_back(betweenVertex);
            indices.push_back(finalVertecies.size() - 1);
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw()
    {
        glBufferData(GL_ARRAY_BUFFER, finalVertecies.size() * sizeof(Vertex), finalVertecies.data(), GL_STATIC_DRAW);
        glBindVertexArray(vao);
        glDrawElements(GL_LINES, finalVertecies.size(), GL_UNSIGNED_INT, 0);
    }
};

class vertexFunc
{

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::vector<Vertex> getVertices(float min, float max, float step)
    {
        std::vector<Vertex> vertices;
        hsl hsl{ 0,1,0.5 };
        auto diff = (max + min) / 2;
        while (min < max)
        {
            auto v = Vertex();
            v.position.x = cos(min);
            v.position.y = sin(min);
            v.position.z = min / 10;
            hsl.shift(min);
            auto rgb = hsl.get_rgb();
            v.color.r = rgb[0];
            v.color.g = rgb[1];
            v.color.b = rgb[2];
            min += step;
            vertices.push_back(v);
        }
        auto v = Vertex();
        v.position.x = cos(min);
        v.position.y = sin(min);
        v.position.z = min / 10;
        hsl.shift(min);
        auto rgb = hsl.get_rgb();
        v.color.r = rgb[0];
        v.color.g = rgb[1];
        v.color.b = rgb[2];
        vertices.push_back(v);
        return vertices;
    }
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;
public:
    vertexFunc()
    {
        vertices = getVertices(0, 6 * M_PI, 0.1f);
        const file_handle file("vf_outp.txt", "w");
        file.printf("%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.position.x, vertex.position.y, vertex.position.z, vertex.color.r, vertex.color.g, vertex.color.b, vertex.texture_coord.x, vertex.texture_coord.y);
        }
    }

    void initDraw()
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            indices.push_back(i);
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw()
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(vao);
        glDrawElements(GL_LINE_STRIP, vertices.size(), GL_UNSIGNED_INT, 0);
    }
};

class twoVarFunc
{
    int size;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Vertex> getVertices(int size, float step, float offset_x, float offset_y)
    {
        std::vector<Vertex> vertices;
        hsl hsl{ 0,1,0.5 };
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                auto v = Vertex();
                v.position.x = -offset_x + i * step;
                v.position.y = -offset_y + j * step;
                v.position.z = sin(v.position.x * v.position.y);
                hsl.shift(v.position.z);
                auto rgb = hsl.get_rgb();
                v.color.r = rgb[0];
                v.color.g = rgb[1];
                v.color.b = rgb[2];
                vertices.push_back(v);
            }
        }
        return vertices;
    }
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;
public:
    twoVarFunc(int size, float step)
    {
        this->size = size;
        const float offset = step * size / 2;
        vertices = getVertices(size, step, offset, offset);
        const file_handle file("f2_outp.txt", "w");
        file.printf("%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.position.x, vertex.position.y, vertex.position.z, vertex.color.r, vertex.color.g, vertex.color.b, vertex.texture_coord.x, vertex.texture_coord.y);
        }
    }
    void initDraw()
    {
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = 0; j < size - 1; j++)
            {
                indices.push_back(i * size + j);
                indices.push_back(i * size + j + 1);
                indices.push_back((i + 1) * size + j);
                indices.push_back((i + 1) * size + j);
                indices.push_back((i + 1) * size + j + 1);
                indices.push_back(i * size + j + 1);
            }
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void draw()
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
};

class drawFromFile
{
    int size;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;
    GLenum mode;
    bool optimizedTriangles;
public:
    drawFromFile(const char* filename, GLenum mode = GL_LINES, bool optimizedTriangles = false)
    {
        std::ifstream file(filename);
        std::string line;
        int size = 0;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            if (size == 0)
            {
                ss >> size;
                continue;
            }
            auto v = Vertex();
            ss >> v.position.x >> v.position.y >> v.position.z >> v.color.r >> v.color.g >> v.color.b >> v.texture_coord.x >> v.texture_coord.y;
            vertices.push_back(v);
            if (!optimizedTriangles)
                indices.push_back(indices.size());
        }
        this->mode = mode;
        this->optimizedTriangles = optimizedTriangles;
    }
    void initDraw()
    {
        if (optimizedTriangles)
        {
            int size = sqrt(vertices.size());

            for (int i = 0; i < size - 1; i++)
            {
                for (int j = 0; j < size - 1; j++)
                {
                    indices.push_back(i * size + j);
                    indices.push_back(i * size + j + 1);
                    indices.push_back((i + 1) * size + j);
                    indices.push_back((i + 1) * size + j);
                    indices.push_back((i + 1) * size + j + 1);
                    indices.push_back(i * size + j + 1);
                }
            }
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    void draw()
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(vao);
        glDrawElements(this->mode, indices.size(), GL_UNSIGNED_INT, 0);
    }
};
