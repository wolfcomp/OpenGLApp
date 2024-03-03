#pragma once
#include "../primitives/Sphere.h"
#include "../primitives/Cone.h"

class Trophy
{
    Sphere topModel;
    Cone botModel;
    glm::vec3 position;
    float yaw;
    float pitch;
    float radius;
    float coneHeight;
    float totHeight; // radius + coneHeight
    ICollision* collision;
    bool isCollected;
    void update_sub_objects();
    void collect();
public:
    Trophy();
    ~Trophy();
    void set_position(const glm::vec3& position);
    void update_shader(Shader* shader);
    Sphere& get_top();
    Cone& get_bot(); 
    void draw();
    glm::vec3 get_position();
};
