#include "Trophy.h"
#include "../collision/AABB.h"

void Trophy::update_sub_objects()
{
    topModel.set_position(position + glm::vec3(0,radius,0));
    botModel.set_position(position - glm::vec3(0,coneHeight,0));
}

void Trophy::collect()
{
    isCollected = true;
}

Trophy::Trophy()
{
    topModel = Sphere();
    botModel = Cone();
    pitch = 0;
    yaw   = 0;
    isCollected = false;
    set_position(glm::vec3(0.0f, 0.0f, 0.0f));
    topModel.set_subdivision(2);
    botModel.set_subdivision(2);
    topModel.set_color(hsl(48, 0.86f, 0.46f));
    botModel.set_color(hsl(48, 0.86f, 0.46f));
    radius = 0.25f;
    coneHeight = .33;
    totHeight = radius + coneHeight;
    topModel.set_radius(radius);
    botModel.set_radius(radius);
    botModel.set_height(coneHeight);
    collision = new AABB(glm::vec3(-radius/2, 0, -radius/2),glm::vec3(radius/2, totHeight/2 + radius * 1.5, radius/2));
    update_sub_objects();
}

void Trophy::set_position(const glm::vec3& position)
{
    this->position = position;
    update_sub_objects();
}

void Trophy::update_shader(Shader* shader) 
{
    topModel.shader = shader;
    botModel.shader = shader;
}

Sphere& Trophy::get_top()
{
    return topModel;
}

Cone& Trophy::get_bot()
{
    return botModel;
}

void Trophy::draw()
{
    if(!isCollected)
    {
        topModel.draw();
        botModel.draw();
        collision->draw_bounds();
    }
}

glm::vec3 Trophy::get_position()
{
    return position;
}

Trophy::~Trophy()
{
    delete collision;
}
