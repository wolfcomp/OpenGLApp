cmake_minimum_required(VERSION 3.21)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(OpenGLApp)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

include_directories(Dependencies/includes)

add_executable(OpenGLApp "OpenGLApp.cpp" "glad.c" ${SOURCES} ${HEADERS})

target_link_directories(OpenGLApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/libs)
target_link_libraries(OpenGLApp PRIVATE "glfw3.lib")
target_link_libraries(OpenGLApp PRIVATE "opengl32.lib")

add_custom_command(TARGET OpenGLApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:OpenGLApp>/shaders)
add_custom_command(TARGET OpenGLApp POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:OpenGLApp>/textures)