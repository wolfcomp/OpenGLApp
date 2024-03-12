#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <sstream>

#include "glm/fwd.hpp"

class Shader
{
    void (*set_params)(const Shader *);

public:
    // the program ID
    unsigned int ID;
    // constructor reads and builds the shader
    Shader(const char *vertex_filename, const char *fragment_filename);
    // use/activate the shader
    void use() const;
    void set_shader_params(void func(const Shader *)) { set_params = func; }
    // utility uniform functions
    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_vec2(const std::string &name, float x, float y) const;
    void set_vec2(const std::string &name, glm::vec2 value) const;
    void set_vec3(const std::string &name, float x, float y, float z) const;
    void set_vec3(const std::string &name, glm::vec3 value) const;
    void set_vec4(const std::string &name, float x, float y, float z, float w) const;
    void set_vec4(const std::string &name, glm::vec4 value) const;
    void set_mat4(const std::string &name, const float *value) const;
};
#endif
