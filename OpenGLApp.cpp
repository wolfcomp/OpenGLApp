#include <cmath>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <windows.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Shader.h"
#define M_PI 3.1415926535897932384626433832795

constexpr int width = 800;
constexpr int height = 800;
float lastX = 400, lastY = 300;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 25000.0f, 0.01f);

struct Vertex
{
    float x, y, z;
    float r, g, b;
    float u, v;
};

Direction direction = Direction::NONE;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_key_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction = direction | Direction::FORWARD;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction = direction | Direction::BACKWARD;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction = direction | Direction::LEFT;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction = direction | Direction::RIGHT;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        direction = direction | Direction::UP;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        direction = direction | Direction::DOWN;
}

void process_mouse_input(GLFWwindow* window, const double x_pos, const double y_pos)
{

    if (firstMouse)
    {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }

    float x_offset = x_pos - lastX;
    float y_offset = lastY - y_pos; // reversed: y ranges bottom to top
    lastX = x_pos;
    lastY = y_pos;

    camera.process_mouse_movement(x_offset, y_offset);
}

void process_direction(double delta_time)
{
    camera.process_keyboard(direction, delta_time);
}

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
            auto v = Vertex{ 0,0,0,0,0,0,0,0 };
            v.x = min - diff;
            v.y = fX(min);
            if (useZ)
                v.z = dfX(min);
            min += step;
            vertices.push_back(v);
        }
        auto v = Vertex{ 0,0,0,0,0,0,0,0 };
        v.x = min - diff;
        v.y = fX(min);
        if (useZ)
            v.z = dfX(min);
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
        fclose(outpFile);
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
            betweenVertex.z = vertex.z;
            betweenVertex.r = vertex.r;
            betweenVertex.g = vertex.g;
            betweenVertex.b = vertex.b;
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
        const file_handle file("vf_outp.txt", "w");
        file.printf("%d points\n", vertices.size());
        for (auto vertex : vertices)
        {
            file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.x, vertex.y, vertex.z, vertex.r, vertex.g, vertex.b, vertex.u, vertex.v);
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
                auto v = Vertex{ 0,0,0,0,0,0,0,0 };
                v.x = -offset_x + i * step;
                v.y = -offset_y + j * step;
                v.z = sin(v.x * v.y);
                hsl.shift(v.z);
                auto rgb = hsl.getRgb();
                v.r = rgb[0];
                v.g = rgb[1];
                v.b = rgb[2];
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
            file.printf("%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", vertex.x, vertex.y, vertex.z, vertex.r, vertex.g, vertex.b, vertex.u, vertex.v);
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
            Vertex v{ 0,0,0,0,0,0,0,0 };
            ss >> v.x >> v.y >> v.z >> v.r >> v.g >> v.b >> v.u >> v.v;
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

int main()
{
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, process_mouse_input);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // uncomment one of these to change the function being used for drawing
    // in order to generate the files the functions below needs to be uncommented once
    // auto f = twoVarFunc(100, 0.1f);
    // auto f = vertexFunc();
    // auto f = func();
    auto f = drawFromFile("vf_outp.txt", GL_LINE_STRIP, false);

    f.initDraw();

    Shader shader("shader.vs", "shader.fs");
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        const double deltaTime = glfwGetTime() - lastTime;
        process_key_input(window);
        process_direction(deltaTime);
        glClear(GL_COLOR_BUFFER_BIT);
        //enable gl wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        shader.use();
        shader.set_mat4("view", value_ptr(camera.get_view_matrix()));
        shader.set_mat4("model", value_ptr(model));
        shader.set_mat4("projection", value_ptr(projection));
        f.draw();
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        direction = Direction::NONE;
        lastTime = glfwGetTime();
    }

    glfwTerminate();

    return 0;
}
