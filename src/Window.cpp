#include "Window.h"
#include <iostream>
#include "ShaderStore.h"
#include "InputProcessing.h"
#include "Light.h"
#include "Shadow.h"
#include "Math.h"
#include "ImGuiManager.h"
#include "windows/InfoWindow.h"
#include "objects/World.h"
#include "objects/primitives/Cube.h"
// #include "objects/debug/Arrow.h"
#include "ModelImport.h"
#include "colliders/ConvexHull.h"
#include "colliders/ColliderRender.h"
#include "Material.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>

#define CAMERA_SPEED 2.5f

constexpr int width = 1600;
constexpr int height = 900;
bool wireframe = false;
double deltaTime = 0.0;
std::chrono::time_point<std::chrono::high_resolution_clock> lastFrame;
hsl pointColor = hsl(0, 0, .8f);
hsl ambientColor = hsl(0, 0, .05f);
glm::vec3 lightPos = glm::vec3(0, 0, 4);
Bezier<glm::vec3> curve{
    glm::vec3(3.5f, 0, 33.5f),
    glm::vec3(3.5f, 0, 17),
    glm::vec3(-15, 0, 33.5),
    glm::vec3(-15, 0, 17)};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};

glm::vec3 cubePositions[] = {
    glm::vec3(2.6f, 5.0f, -15.4f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(-4.5f, 0, -10.2f)};

Camera camera;
InputProcessing input;
ShadowProcessor shadowProcessor;
ImGuiManager imguiManager;
LightManager lightManager;
World world;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    input.change_aspect(static_cast<float>(width), static_cast<float>(height));
}

void move_character(const glm::vec3 &direction)
{
    auto pos = camera.get_pos();
    pos += camera.get_movement(direction) * (float)(CAMERA_SPEED * deltaTime);
    camera.set_position(pos);
}

int Window::init()
{
    input.change_aspect(width, height);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "OpenGLApp", nullptr, nullptr);
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

    GLint maxTessLevel;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);
    // std::cout << "Max tessellation level: " << maxTessLevel << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 410");

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x_pos, double y_pos)
                             { input.process_mouse_move(window, x_pos, y_pos); });
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods)
                               { input.process_mouse_button(window, button, action, mods); });
    glfwSetScrollCallback(window, [](GLFWwindow *window, double x_offset, double y_offset)
                          { input.process_mouse_scroll(window, x_offset, y_offset); });
    glfwSetCharCallback(window, [](GLFWwindow *window, unsigned int codepoint)
                        { ImGui_ImplGlfw_CharCallback(window, codepoint); });

    init_listeners();

    Mesh::setup();

    return 0;
}

void Window::init_listeners()
{
    input.attach_keyboard_listener(
        GLFW_KEY_F, []()
        { wireframe = !wireframe; },
        false);
    input.attach_keyboard_listener(
        GLFW_KEY_W, []()
        { move_character(glm::vec3(0, 0, -1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_S, []()
        { move_character(glm::vec3(0, 0, 1)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_A, []()
        { move_character(glm::vec3(-1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_D, []()
        { move_character(glm::vec3(1, 0, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_SPACE, []()
        { move_character(glm::vec3(0, 1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_LEFT_CONTROL, []()
        { move_character(glm::vec3(0, -1, 0)); },
        true);
    input.attach_keyboard_listener(
        GLFW_KEY_C, []()
        { world.toggle_collider_render(); },
        false);
    input.attach_mouse_listener([](MouseInput input)
                                { camera.process_mouse(input.x_pos_offset, input.y_pos_offset); });
}

void Window::create_objects()
{
    shadowProcessor.init();

    DirectionalLight *dirLight = new DirectionalLight();
    PointLight *light0 = new PointLight();
    PointLight *light1 = new PointLight();
    PointLight *light2 = new PointLight();
    PointLight *light3 = new PointLight();
    SpotLight *spotLight = new SpotLight();

    lightManager.add_light(dirLight);
    lightManager.add_light(light0);
    lightManager.add_light(light1);
    lightManager.add_light(light2);
    lightManager.add_light(light3);
    lightManager.add_light(spotLight);

    light0->index = 0;
    light0->position = pointLightPositions[0];
    light0->diffuse = pointColor;
    light0->ambient = ambientColor;
    light0->specular = glm::vec3(1);
    light0->constant = 1.0f;
    light0->linear = 0.09f;
    light0->quadratic = 0.032f;

    *light1 = *light2 = *light3 = *light0;
    light1->index = 1;
    light2->index = 2;
    light3->index = 3;
    light1->position = pointLightPositions[1];
    light2->position = pointLightPositions[2];
    light3->position = pointLightPositions[3];

    dirLight->ambient = hsl(120, .4f, .2f);
    dirLight->diffuse = hsl(0, 0, .8f);
    dirLight->specular = glm::vec3(1);
    dirLight->direction = normalize(glm::vec3(-0.7f, -1.0f, -0.3f));
    dirLight->position = lightPos;

    spotLight->position = pointLightPositions[0];
    spotLight->direction = normalize(glm::vec3(0, 0, -1));
    spotLight->constant = 1.0f;
    spotLight->linear = 0.09f;
    spotLight->quadratic = 0.032f;
    spotLight->ambient = hsl(0, 0, .05f);
    spotLight->diffuse = hsl(0, 0, .8f);
    spotLight->specular = glm::vec3(1);
    spotLight->cutOff = glm::cos(glm::radians(12.5f));
    spotLight->outerCutOff = glm::cos(glm::radians(17.5f));

    imguiManager.add_window(new InfoWindow(&camera));

    auto cube = new Cube();

    cube->material = new TextureMaterial();
    dynamic_cast<TextureMaterial *>(cube->material)->load_texture("container", "container.png");
    cube->material->shader = ShaderStore::get_shader("default");
    cube->material->shadow_shader = ShaderStore::get_shader("shadow");

    world.add_mesh(cube);

    auto testFbx = ModelImport::load_model("assets/models/test.fbx");
    testFbx->material = new ColorMaterial();
    dynamic_cast<ColorMaterial *>(testFbx->material)->color = glm::vec3(.7f);
    testFbx->material->shader = ShaderStore::get_shader("default");
    auto testFbxCollider = new ConvexHull(testFbx);
    testFbx->collider = testFbxCollider;
    testFbx->add_child(new ColliderRender<ConvexHull>(testFbxCollider));

    world.add_mesh(testFbx);

    // auto arrow = new Arrow(dirLight->direction);
    // arrow->material = new ColorMaterial();
    // dynamic_cast<ColorMaterial *>(arrow->material)->color = glm::vec3(1, 1, 0);
    // arrow->material->shader = ShaderStore::get_shader("noLight");
    // arrow->position = dirLight->position;

    // world.add_mesh(arrow);

    // auto y_axis = new Arrow(glm::vec3(0, 1, 0));
    // y_axis->material = new ColorMaterial();
    // dynamic_cast<ColorMaterial *>(y_axis->material)->color = glm::vec3(0, 0, 1);
    // y_axis->material->shader = ShaderStore::get_shader("noLight");
    // y_axis->position = glm::vec3(0, 0, 0);

    // world.add_mesh(y_axis);

    // auto x_axis = new Arrow(glm::vec3(1, 0, 0));
    // x_axis->material = new ColorMaterial();
    // dynamic_cast<ColorMaterial *>(x_axis->material)->color = glm::vec3(1, 0, 0);
    // x_axis->material->shader = ShaderStore::get_shader("noLight");
    // x_axis->position = glm::vec3(0, 0, 0);

    // world.add_mesh(x_axis);

    // auto z_axis = new Arrow(glm::vec3(0, 0, 1));
    // z_axis->material = new ColorMaterial();
    // dynamic_cast<ColorMaterial *>(z_axis->material)->color = glm::vec3(0, 1, 0);
    // z_axis->material->shader = ShaderStore::get_shader("noLight");
    // z_axis->position = glm::vec3(0, 0, 0);

    // world.add_mesh(z_axis);

    ShaderStore::set_shader_params(
        [](const Shader *shad)
        {
            camera.set_shader(shad);
            lightManager.set_shader(shad);
            input.set_shader(shad);
            shad->set_float("gammaCorrection", 2.2f);
            shadowProcessor.bind_depth_map(shad);
            auto dir_light = lightManager.get_directional_light();
            shad->set_mat4("lightSpaceMatrix", shadowProcessor.get_light_space_matrix(dir_light->position, dir_light->direction));
        });
}

void Window::update() const
{
    deltaTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - lastFrame).count();
    lastFrame = std::chrono::high_resolution_clock::now();
    input.process_keyboard(window, 0);
    imguiManager.render();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(5);
    // enable gl wireframe mode
    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shadowProcessor.bind_buffer();
    world.draw_shadow(glm::mat4(1));
    shadowProcessor.unbind_buffer(input.get_screen_size());
    glCullFace(GL_FRONT);
    world.draw(glm::mat4(1));
    imguiManager.render_draw_data();
    glCullFace(GL_BACK);

    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::render() const
{
    // first run the update
    update();
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(window);
}

Window::~Window()
{
    world.cleanup();
    imguiManager.cleanup();
    lightManager.cleanup();
    input.cleanup();
    shadowProcessor.cleanup();
    ShaderStore::remove_all_shaders();
    glfwDestroyWindow(window);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}