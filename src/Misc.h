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

    void printf(_Printf_format_string_ const char* const format, ...) const
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
                vertex.normal.r = 1;
            else
                vertex.normal.g = 1;
            vertices[i] = vertex;
        }
        FILE* outpFile;
        fopen_s(&outpFile, "f_outp.txt", "w");
        fprintf(outpFile, "%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.position.x, vertex.position.y, vertex.position.z, vertex.normal.r, vertex.normal.g, vertex.normal.b, vertex.texture_coord.x, vertex.texture_coord.y);
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
            betweenVertex.normal.r = vertex.normal.r;
            betweenVertex.normal.g = vertex.normal.g;
            betweenVertex.normal.b = vertex.normal.b;
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
        glDrawElements(GL_LINES, finalVertecies.size(), GL_UNSIGNED_INT, nullptr);
    }
};

class vertexFunc
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::vector<Vertex> getVertices(float min, float max, float step)
    {
        std::vector<Vertex> vertices;
        hsl hsl{0, 1, 0.5};
        auto diff = (max + min) / 2;
        while (min < max)
        {
            auto v = Vertex();
            v.position.x = cos(min);
            v.position.y = sin(min);
            v.position.z = min / 10;
            hsl.shift(min);
            auto rgb = hsl.get_rgb();
            v.normal.r = rgb[0];
            v.normal.g = rgb[1];
            v.normal.b = rgb[2];
            min += step;
            vertices.push_back(v);
        }
        auto v = Vertex();
        v.position.x = cos(min);
        v.position.y = sin(min);
        v.position.z = min / 10;
        hsl.shift(min);
        auto rgb = hsl.get_rgb();
        v.normal.r = rgb[0];
        v.normal.g = rgb[1];
        v.normal.b = rgb[2];
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
            file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.position.x, vertex.position.y, vertex.position.z, vertex.normal.r, vertex.normal.g, vertex.normal.b, vertex.texture_coord.x, vertex.texture_coord.y);
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
        glDrawElements(GL_LINE_STRIP, vertices.size(), GL_UNSIGNED_INT, nullptr);
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
        hsl hsl{0, 1, 0.5};
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
                v.normal.r = rgb[0];
                v.normal.g = rgb[1];
                v.normal.b = rgb[2];
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
            file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.position.x, vertex.position.y, vertex.position.z, vertex.normal.r, vertex.normal.g, vertex.normal.b, vertex.texture_coord.x, vertex.texture_coord.y);
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
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
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
    bool optimized;

public:
    drawFromFile(const char* filename, GLenum mode = GL_LINES, bool optimized = false, bool scale_to_negative_one_one = true)
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
            ss >> v.position.x >> v.position.y >> v.position.z >> v.normal.r >> v.normal.g >> v.normal.b >> v.texture_coord.x >> v.texture_coord.y;
            vertices.push_back(v);
            if (!optimized)
                indices.push_back(indices.size());
        }
        if (scale_to_negative_one_one)
        {
            float minx = vertices[0].position.x;
            float miny = vertices[0].position.y;
            float maxx = vertices[0].position.x;
            float maxy = vertices[0].position.y;
            for (auto vertex : vertices)
            {
                if (vertex.position.x < minx)
                    minx = vertex.position.x;
                if (vertex.position.y < miny)
                    miny = vertex.position.y;
                if (vertex.position.x > maxx)
                    maxx = vertex.position.x;
                if (vertex.position.y > maxy)
                    maxy = vertex.position.y;
            }

            float xscale, yscale;
            xscale = abs(minx) > abs(maxx) ? abs(minx) : abs(maxx);
            yscale = abs(miny) > abs(maxy) ? abs(miny) : abs(maxy);

            float scale = xscale > yscale ? xscale : yscale;

            for (auto& vert : vertices)
            {
                vert.position.x = vert.position.x / scale;
                vert.position.y = vert.position.y / scale;
            }
        }
        this->mode = mode;
        this->optimized = optimized;
    }

    void initDraw()
    {
        if (optimized)
        {
            if (mode == GL_LINES)
            {
                for (int i = 0; i < vertices.size() - 1; i++)
                {
                    indices.push_back(i);
                    indices.push_back(i + 1);
                }
            }
            else if (mode == GL_TRIANGLES)
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
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        // ReSharper disable CppCStyleCast
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        // ReSharper restore CppCStyleCast
    }

    void draw()
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(vao);
        glDrawElements(this->mode, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
};
