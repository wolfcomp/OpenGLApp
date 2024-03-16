#pragma once

#include <glm/fwd.hpp>

class Shader;

class ShadowProcessor
{
    unsigned int depth_map_fbo;
    unsigned int depth_map;

    const unsigned int SHADOW_WIDTH = 1024;
    const unsigned int SHADOW_HEIGHT = 1024;

public:
    ShadowProcessor() = default;
    ShadowProcessor(const ShadowProcessor &) = delete;
    ShadowProcessor(ShadowProcessor &&) = delete;
    ShadowProcessor &operator=(const ShadowProcessor &) = delete;
    ShadowProcessor &operator=(ShadowProcessor &&) = delete;

    void init();
    void bind_buffer();
    void unbind_buffer(const glm::vec2 &window_size);
    void bind_depth_map(const Shader *shader);
    glm::mat4 get_light_space_matrix(const glm::vec3 &character_pos, const glm::vec3 &light_dir, const glm::vec3 &character_dir);
};