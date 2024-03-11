cmake_minimum_required(VERSION 3.21)

project(OpenGLApp
    VERSION 1.0
    DESCRIPTION "OpenGL Application"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

add_subdirectory(Dependencies/includes/glm)
add_subdirectory(Dependencies/includes/glfw)
add_subdirectory(Dependencies/includes/KHR)
add_subdirectory(Dependencies/includes/glad)
add_dependencies(OpenGLApp glm)
add_dependencies(OpenGLApp glfw)
add_dependencies(OpenGLApp glad)
add_dependencies(OpenGLApp KHR)