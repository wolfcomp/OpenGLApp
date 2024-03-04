#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>

#include "src/InputProcessing.h"
#include "src/Misc.h"
#include "src/ObjectBuffer.h"
#include "src/ShaderStore.h"
#include "src/TimeManager.h"
#include "src/objects/Character.h"
#include "src/objects/House.h"
#include "src/primitives/Cube.h"
#include "src/primitives/Plane.h"
#include "src/objects/Trophy.h"
#include <random>

#include "glm/gtx/rotate_vector.hpp"

constexpr int width = 1600;
constexpr int height = 900;
double lastX, lastY;
bool firstMouse = true;
int subdivision = 0;
int lastSubdivision = 0;
bool wireframe = false;

float staticCamYaw = 0.f;
float staticCamPitch = 0.f;

InputProcessing input;
ObjectBuffer objBuffer;
Character character;
Character npc;
//Having them in vectors or arrays didn't work, and caused the program to crash on draw
Trophy trophy0, trophy1, trophy2, trophy3, trophy4, trophy5, trophy6, trophy7;
float offsetTrop;


double npcProgress = 0;
float npcSpeed = 100;

BSpline<glm::vec3> splineObj;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    input.change_aspect(static_cast<float>(width), static_cast<float>(height));
}

void increase_subdivision()
{
    if (subdivision < 5)
    {
        subdivision++;
    }
}

void decrease_subdivision()
{
    if (subdivision > 0)
    {
        subdivision--;
    }
}

void process_mouse_input(GLFWwindow* window, const double x_pos, const double y_pos)
{
    if (firstMouse)
    {
        lastX = x_pos;
        lastY = y_pos;
        firstMouse = false;
    }

    const double xOffset = x_pos - lastX;
    const double yOffset = lastY - y_pos; // reversed: y ranges bottom to top
    lastX = x_pos;
    lastY = y_pos;

    character.process_mouse_movement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    character.process_mouse_scroll(y_offset);
}

void set_trophy_positions(float offset_trop)
{
    trophy0.set_position(glm::vec3(0 + 10, .75, 0));
    trophy1.set_position(glm::vec3(offset_trop + 10, .75, offset_trop));
    trophy2.set_position(glm::vec3(offset_trop * 2 + 10, .75, offset_trop * 2));
    trophy3.set_position(glm::vec3(offset_trop * 3 + 10, .75, offset_trop * 3));
    trophy4.set_position(glm::vec3(offset_trop * 4 + 10, .75, offset_trop * 4));
    trophy5.set_position(glm::vec3(offset_trop * 5 + 10, .75, offset_trop * 5));
    trophy6.set_position(glm::vec3(offset_trop * 6 + 10, .75, offset_trop * 6));
    trophy7.set_position(glm::vec3(offset_trop * 7 + 10, .75, offset_trop * 7));
}

void instantiate_trophies(float& offset_trop)
{
    trophy0.update_shader((ShaderStore::get_shader("default")));
    trophy1.update_shader((ShaderStore::get_shader("default")));
    trophy2.update_shader((ShaderStore::get_shader("default")));
    trophy3.update_shader((ShaderStore::get_shader("default")));
    trophy4.update_shader((ShaderStore::get_shader("default")));
    trophy5.update_shader((ShaderStore::get_shader("default")));
    trophy6.update_shader((ShaderStore::get_shader("default")));
    trophy7.update_shader((ShaderStore::get_shader("default")));
    offset_trop = 3.f;
    set_trophy_positions(offset_trop);
    objBuffer.add_object(&trophy0);
    objBuffer.add_object(&trophy1);
    objBuffer.add_object(&trophy2);
    objBuffer.add_object(&trophy3);
    objBuffer.add_object(&trophy4);
    objBuffer.add_object(&trophy5);
    objBuffer.add_object(&trophy6);
    objBuffer.add_object(&trophy7);
}

void setup_npc_path(float offsetTrop)
{
    auto b1 = Bezier<glm::vec3>(trophy0.get_position(), trophy0.get_position() + glm::vec3(offsetTrop, 0, 0),
                                trophy1.get_position() - glm::vec3(offsetTrop, 0, 0), trophy1.get_position());

    auto b2 = Bezier<glm::vec3>(trophy1.get_position(), trophy1.get_position() + glm::vec3(offsetTrop * 2, 0, 0),
                                trophy2.get_position() - glm::vec3(offsetTrop * 2, 0, 0), trophy2.get_position());

    auto b3 = Bezier<glm::vec3>(trophy2.get_position(), trophy2.get_position() + glm::vec3(offsetTrop * 3, 0, 0),
                                trophy3.get_position() - glm::vec3(offsetTrop * 3, 0, 0), trophy3.get_position());

    auto b4 = Bezier<glm::vec3>(trophy3.get_position(), trophy3.get_position() + glm::vec3(offsetTrop * 4, 0, 0),
                                trophy4.get_position() - glm::vec3(offsetTrop * 4, 0, 0), trophy4.get_position());

    auto b5 = Bezier<glm::vec3>(trophy4.get_position(), trophy4.get_position() + glm::vec3(offsetTrop * 5, 0, 0),
                                trophy5.get_position() - glm::vec3(offsetTrop * 5, 0, 0), trophy5.get_position());

    auto b6 = Bezier<glm::vec3>(trophy5.get_position(), trophy5.get_position() + glm::vec3(offsetTrop * 6, 0, 0),
                                trophy6.get_position() - glm::vec3(offsetTrop * 6, 0, 0), trophy6.get_position());

    auto b7 = Bezier<glm::vec3>(trophy6.get_position(), trophy6.get_position() + glm::vec3(offsetTrop * 7, 0, 0),
                                trophy7.get_position() - glm::vec3(offsetTrop * 7, 0, 0), trophy7.get_position());

    auto b8 = Bezier<glm::vec3>(trophy7.get_position(), trophy7.get_position() + glm::vec3(offsetTrop * 8, 0, 0),
                                trophy0.get_position() - glm::vec3(offsetTrop * 8, 0, 0), trophy0.get_position());

    splineObj.points.clear();
    splineObj.add_point(0, b1);
    splineObj.add_point(.15, b2);
    splineObj.add_point(.3, b3);
    splineObj.add_point(.45, b4);
    splineObj.add_point(.6, b5);
    splineObj.add_point(.75, b6);
    splineObj.add_point(.9, b7);
    splineObj.add_point(1, Bezier<glm::vec3>(trophy7.get_position(), trophy7.get_position(), trophy7.get_position(),
                                             trophy7.get_position()));
}

void npc_movement()
{
    npcProgress += TimeManager::get_delta_time() * npcSpeed;
    if (npcProgress >= 2)
        npcProgress = 0;
    auto p = splineObj(abs(npcProgress - 1));
    npc.set_position(p);
}

void move_character(const glm::vec3& direction)
{
    character.update_position(direction, TimeManager::get_delta_time(), objBuffer);
}

void invert_curve()
{
    offsetTrop = -offsetTrop;
    setup_npc_path(offsetTrop);
}

int main()
{
    input.change_aspect(width, height);
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
    // ReSharper disable once CppCStyleCast
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, process_mouse_input);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    input.attach_keyboard_listener(GLFW_KEY_UP, increase_subdivision, false);
    input.attach_keyboard_listener(GLFW_KEY_DOWN, decrease_subdivision, false);
    input.attach_keyboard_listener(GLFW_KEY_F, []() { wireframe = !wireframe; }, false);
    input.attach_keyboard_listener(GLFW_KEY_W, []() { move_character(glm::vec3(1, 0, 0)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_S, []() { move_character(glm::vec3(-1, 0, 0)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_A, []() { move_character(glm::vec3(0, 0, -1)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_D, []() { move_character(glm::vec3(0, 0, 1)); }, true);
    input.attach_keyboard_listener(GLFW_KEY_I, []() { invert_curve(); }, false);

    ShaderStore::add_shader("default", "shader.vs", "shader.fs");

    objBuffer.init_buffers();

    auto plane = Plane();

    plane.shader = ShaderStore::get_shader("default");

    plane.set_size(glm::vec2(100));
    plane.set_color(hsl(0, .5, .5));

    objBuffer.add_object(&plane);

    auto house = House();

    house.shader = ShaderStore::get_shader("default");
    house.set_scale(glm::vec3(2));
    house.set_position(glm::vec3(5, 0, 10));
    house.set_rotation(65.f);

    objBuffer.add_object(&house);

    character.set_position(glm::vec3(0, 0.75f, 0));
    character.set_shader(ShaderStore::get_shader("default"));

    npc.set_shader(ShaderStore::get_shader("default"));

    instantiate_trophies(offsetTrop);

    setup_npc_path(offsetTrop); // has to be after trophies are placed out

    TimeManager::set_last_frame(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        TimeManager::set_current_frame(glfwGetTime());
        TimeManager::set_delta_time((TimeManager::get_current_frame() - TimeManager::get_last_frame()) / 1000);
        TimeManager::set_last_frame(TimeManager::get_current_frame());
        auto curDoorLerp = TimeManager::get_door_lerp();
        input.process_keyboard(window, TimeManager::get_delta_time());
        character.check_overlap(objBuffer);
        ShaderStore::set_shader_params([](const Shader* shad)
        {
            input.set_shader(shad);
            character.update_shader(shad);
        });
        if (lastSubdivision != subdivision)
        {
            lastSubdivision = subdivision;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPointSize(5);
        //enable gl wireframe mode
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        npc_movement();
        objBuffer.draw();
        character.draw();
        npc.draw();

        if(abs(curDoorLerp - TimeManager::get_door_lerp()) < std::numeric_limits<double>::epsilon() && !house.is_inside())
        {
            auto doorLerp = curDoorLerp - TimeManager::get_delta_time() * 1000;
            doorLerp = clamp(doorLerp, 0.0, 1.0);
            TimeManager::set_door_lerp(doorLerp);
            house.set_door_rotation(lerp(0, -90, doorLerp));
            house.collision->should_overlap = false;
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
