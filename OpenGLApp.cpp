#include <algorithm>
#include <cmath>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <windows.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Shader.h"
#define M_PI 3.1415926535897932384626433832795

constexpr int width = 800;
constexpr int height = 800;

struct Vertex
{
    float x, y, z;
    float r, g, b;
    float u, v;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

class hsl
{
    template <typename T>
    T clamp(T i, T min, T max)
    {
        if (i < min)
            return min;
        if (i > max)
            return max;
        return i;
    }

    float getModif(float time, float (*func)(float))
    {
        auto t = func(time);
        if (t < 0)
            t = -t;
        return 0.25f * t + 0.25f;
    }

    template <typename T>
    T mod(T i, int mod)
    {
        return i - mod * static_cast<int>(i / mod);
    }
public:
    hsl(float h, float s, float l) : h(h), s(s), l(l) {}
    int h;
    float s;
    float l;
    float rgb[3];
    void shift(float time)
    {
        h = mod(time * 100, 360.0f);
    }
    void fromTime(float time)
    {
        h = static_cast<int>(time) % 360;
        s = 1;
        l = clamp(getModif(time, sin), 0.25f, 0.75f);
    }
    float* getRgb()
    {
        float c = (1.0f - abs(2.0f * l - 1.0f)) * s;
        float x = c * (1.0f - abs(mod(h / 60.0f, 2) - 1.0f));
        float m = l - c / 2;
        float r, g, b;
        if (h < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (h < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (h < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (h < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (h < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else
        {
            r = c;
            g = 0;
            b = x;
        }
        r += m;
        g += m;
        b += m;
        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
        return rgb;
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
    std::vector<Vertex> getVertices(float min, float max, float step)
    {
        std::vector<Vertex> vertices;
        auto diff = (max + min) / 2;
        while (min < max)
        {
            auto v = Vertex{ 0,0,0,0,0,0,0,0 };
            v.x = min - diff;
            v.y = fX(min);
            min += step;
            vertices.push_back(v);
        }
        auto v = Vertex{ 0,0,0,0,0,0,0,0 };
        v.x = min - diff;
        v.y = fX(min);
        vertices.push_back(v);
        return vertices;
    }
    unsigned int vao;
    unsigned int ebo;
    unsigned int vbo;
public:
    func()
    {
        vertices = getVertices(-.3f, 1.3f, 0.1f);
        for (int i = 0; i < vertices.size() - 1; i++)
        {
            auto vertex = vertices[i];
            auto nextVertex = vertices[i + 1];
            if (nextVertex.y < vertex.y)
                vertex.r = 1;
            else
                vertex.g = 1;
            vertices[i] = vertex;
        }
        FILE* outpFile;
        fopen_s(&outpFile, "f_outp.txt", "w");
        fprintf(outpFile, "%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.x, vertex.y, vertex.z, vertex.r, vertex.g, vertex.b, vertex.u, vertex.v);
        }
    }

    void initDraw()
    {
        for (int i = 0; i < vertices.size() - 1; i++)
        {
            auto vertex = vertices[i];
            auto nextVertex = vertices[i + 1];
            Vertex betweenVertex{ 0,0,0,0,0,0,0,0 };
            finalVertecies.push_back(vertex);
            indices.push_back(finalVertecies.size() - 1);
            betweenVertex.x = nextVertex.x;
            betweenVertex.y = nextVertex.y;
            betweenVertex.r = vertex.r;
            betweenVertex.g = vertex.g;
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
        glDrawElements(GL_LINE, finalVertecies.size(), GL_UNSIGNED_INT, 0);
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
            auto v = Vertex{ 0,0,0,0,0,0,0,0 };
            v.x = cos(min);
            v.y = sin(min);
            v.z = min / 10;
            hsl.shift(min);
            auto rgb = hsl.getRgb();
            v.r = rgb[0];
            v.g = rgb[1];
            v.b = rgb[2];
            min += step;
            vertices.push_back(v);
        }
        auto v = Vertex{ 0,0,0,0,0,0,0,0 };
        v.x = cos(min);
        v.y = sin(min);
        v.z = min / 10;
        hsl.shift(min);
        auto rgb = hsl.getRgb();
        v.r = rgb[0];
        v.g = rgb[1];
        v.b = rgb[2];
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
        FILE* outpFile;
        fopen_s(&outpFile, "vf_outp.txt", "w");
        fprintf(outpFile, "%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            fprintf(outpFile, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.x, vertex.y, vertex.z, vertex.r, vertex.g, vertex.b, vertex.u, vertex.v);
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

int main()
{
    auto f = func();
    auto vf = vertexFunc();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLApp", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    f.initDraw();
    vf.initDraw();

    Shader shader("shader.vs", "shader.fs");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();
        // f.draw();
        vf.draw();
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
