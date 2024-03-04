#include "House.h"

#include "../HSL.h"
#include "../Math.h"
#include "../TimeManager.h"
#include "../collision/OBB.h"
#include "glm/gtx/rotate_vector.hpp"

void handle_inside(IObject* self_obj, ICollision* self, ICollision* other)
{
    const auto house = dynamic_cast<House*>(self_obj);
    house->set_inside(true);
}

House::House()
{
    position = glm::vec3(0.f, 0.f, 0.f);
    angle = 0;
    scale = glm::vec3(1.f);
    doorOffset = .35f;
    collision = new OBB();
    doorOpen = new OBB();
    rightDoorGuard = new OBB();
    leftDoorGuard = new OBB();
    backInside = new OBB();
    leftInside = new OBB();
    rightInside = new OBB();
    frontLeftInside = new OBB();
    frontRightInside = new OBB();
    collision->parent = this;
    doorOpen->parent = this;
    frontLeftInside->should_overlap = frontRightInside->should_overlap = backInside->should_overlap = leftInside->should_overlap = rightInside->should_overlap = doorOpen->should_overlap = true;
    collision->on_collision = handle_inside;
    doorOpen->on_collision = [](IObject* self_obj, ICollision* self, ICollision* other)
        {
            const auto house = dynamic_cast<House*>(self_obj);
            auto doorLerp = TimeManager::get_door_lerp();
            doorLerp += TimeManager::get_delta_time() * 1000;
            doorLerp = clamp(doorLerp, 0, 1);
            house->set_door_rotation(lerp(0, -90, doorLerp));
            TimeManager::set_door_lerp(doorLerp);
            house->collision->should_overlap = true;
            house->set_inside(false);
        };
    generate_vertices();

    //Cube inserted into the House
    cube.set_scale(glm::vec3(0.2));
    cube.set_position(glm::vec3(0.7,0.1,3.2));
    cube.set_color(hsl(179, .62, .50));

    //second cube inserted into the house
    cube2.set_scale(glm::vec3(0.4));
    cube2.set_position(glm::vec3(-0.5, 0.2, 2));
    cube2.set_color(hsl(120, .77, .49));

    //Sphere inserted into the house
    sphere.set_radius(.4);
    sphere.set_position(glm::vec3(-.8, 0.2, 0.7));
    sphere.set_color(hsl(288, 1, .49));
    sphere.set_subdivision(4);
}

void House::generate_vertices()
{
    Vertex vertex;
    hsl wallColor(0, .4f, .52f);
    hsl roofColor(0, .4f, .3f);
    vertex.color = wallColor.get_rgb_vec3();
    vertex.position = glm::vec3(0, 0.f, 0);
    vertices.push_back(vertex); // 0
    vertex.position = glm::vec3(0, 1.5f, 0);
    vertices.push_back(vertex); // 1
    vertex.position = glm::vec3(0, 1.5f, 4);
    vertices.push_back(vertex); // 2
    vertex.position = glm::vec3(0, 0.f, 4);
    vertices.push_back(vertex); // 3
    vertex.position.z *= doorOffset;
    vertex.position.z += 0.25f;
    vertices.push_back(vertex); // 4
    vertex.position.z -= 0.5f;
    vertices.push_back(vertex); // 5
    vertex.position.y = 1;
    vertices.push_back(vertex); // 6
    vertex.position.z += 0.5f;
    vertices.push_back(vertex); // 7
    vertex.position = glm::vec3(1.5f, 0.f, 0);
    vertices.push_back(vertex); // 8
    vertex.position = glm::vec3(1.5f, 1.5f, 0);
    vertices.push_back(vertex); // 9
    vertex.position.x /= 2;
    vertex.position.y *= 1.5f;
    vertices.push_back(vertex); // 10
    vertex.position = glm::vec3(1.5f, 0.f, 4);
    vertices.push_back(vertex); // 11
    vertex.position = glm::vec3(1.5f, 1.5f, 4);
    vertices.push_back(vertex); // 12
    vertex.position.x /= 2;
    vertex.position.y *= 1.5f;
    vertices.push_back(vertex); // 13
    vertex = vertices[1];
    vertex.color = roofColor.get_rgb_vec3();
    vertex.position -= glm::vec3(0.25f, 0.25f, 0.25f);
    vertices.push_back(vertex); // 14
    vertex = vertices[2];
    vertex.color = roofColor.get_rgb_vec3();
    vertex.position -= glm::vec3(0.25f, 0.25f, -0.25f);
    vertices.push_back(vertex); // 15
    vertex = vertices[9];
    vertex.color = roofColor.get_rgb_vec3();
    vertex.position -= glm::vec3(-0.25f, 0.25f, 0.25f);
    vertices.push_back(vertex); // 16
    vertex = vertices[12];
    vertex.color = roofColor.get_rgb_vec3();
    vertex.position -= glm::vec3(-0.25f, 0.25f, -0.25f);
    vertices.push_back(vertex); // 17
    vertex = vertices[10];
    vertex.color = roofColor.get_rgb_vec3();
    vertex.position -= glm::vec3(0, 0, 0.25f);
    vertices.push_back(vertex); // 18
    vertex = vertices[13];
    vertex.color = roofColor.get_rgb_vec3();
    vertex.position -= glm::vec3(0, 0, -0.25f);
    vertices.push_back(vertex); // 19

    indices = {
        // front
        0, 6, 1, 0, 5, 6,
        4, 3, 2, 4, 2, 7,
        1, 6, 2, 6, 7, 2,

        // left
        0, 1, 8, 8, 1, 9,
        9, 1, 10,

        // right
        3, 11, 2, 2, 11, 12,
        2, 12, 13,

        // back
        11, 8, 12, 8, 9, 12,

        // roof front
        14, 15, 18, 18, 15, 19,

        // roof back
        16, 17, 19, 19, 16, 18
    };

    dynamic_cast<OBB*>(collision)->points = {
        glm::vec2(0, 0),
        glm::vec2(0, 4),
        glm::vec2(1.5f, 0),
        glm::vec2(1.5f, 4)
    };
    doorOpen->points = {
        glm::vec2(0, 0),
        glm::vec2(0, .5f),
        glm::vec2(-1, 0),
        glm::vec2(-1, .5f)
    };
    leftDoorGuard->points = {
        glm::vec2(0, 0),
        glm::vec2(0, -.25f),
        glm::vec2(-.25f, 0),
        glm::vec2(-.25f, -.25f)
    };
    rightDoorGuard->points = {
        glm::vec2(0, .5f),
        glm::vec2(0, .75f),
        glm::vec2(-.25f, .5f),
        glm::vec2(-.25f, .75f)
    };
    for (auto& vertex : vertices)
    {
        vertex.position = rotateY(vertex.position, glm::radians(angle));
        vertex.position *= scale;
        vertex.position += position;
    }
    ProperVertexes = vertices;
    OffsetVertexes = vertices;
    OffsetVertexes[11].position += (vertices[12].position - vertices[11].position) / 2.f;
    OffsetVertexes[8] = OffsetVertexes[11];
    OffsetVertexes[9] = vertices[12];
    OffsetVertexes[0] = vertices[1];
    OffsetVertexes[16] = vertices[19];
    OffsetVertexes[17] = vertices[18];
    for (auto& point : dynamic_cast<OBB*>(collision)->points)
    {
        point = rotate(point, glm::radians(-angle));
        point *= glm::vec2(scale.x, scale.z);
        point += glm::vec2(position.x, position.z);
    }
    vertex = vertices[5];
    for (int i = 0; i < doorOpen->points.size(); ++i)
    {
        doorOpen->points[i] = rotate(doorOpen->points[i], glm::radians(-angle));
        doorOpen->points[i] *= glm::vec2(scale.x, scale.z);
        doorOpen->points[i] += glm::vec2(vertex.position.x, vertex.position.z);
        leftDoorGuard->points[i] = rotate(leftDoorGuard->points[i], glm::radians(-angle));
        leftDoorGuard->points[i] *= glm::vec2(scale.x, scale.z);
        leftDoorGuard->points[i] += glm::vec2(vertex.position.x, vertex.position.z);
        rightDoorGuard->points[i] = rotate(rightDoorGuard->points[i], glm::radians(-angle));
        rightDoorGuard->points[i] *= glm::vec2(scale.x, scale.z);
        rightDoorGuard->points[i] += glm::vec2(vertex.position.x, vertex.position.z);
    }
    door.set_position(vertex.position);
    door.set_rotation(angle + doorAngle);
    auto size = (.25 / 2) * scale.x;
    auto leftPos = leftDoorGuard->points[0] + (leftDoorGuard->points[3] - leftDoorGuard->points[0]) / 2.f;
    auto rightPos = rightDoorGuard->points[0] + (rightDoorGuard->points[3] - rightDoorGuard->points[0]) / 2.f;
    leftSide.set_scale(glm::vec3(size, size, size));
    rightSide.set_scale(glm::vec3(size, size, size));
    leftSide.set_position(glm::vec3(leftPos.x, position.y + size, leftPos.y));
    rightSide.set_position(glm::vec3(rightPos.x, position.y + size, rightPos.y));
    leftSide.set_rotation(glm::vec3(0, glm::radians(angle), 0));
    rightSide.set_rotation(glm::vec3(0, glm::radians(angle), 0));
    auto x0 = leftDoorGuard->points[1];
    auto x1 = rightDoorGuard->points[1];
    auto offsetY = rotate(glm::vec2(.5, 0), glm::radians(-angle));
    auto offsetX = rotate(glm::vec2(0, .5), glm::radians(-angle));
    frontLeftInside->points = {
        glm::vec2(vertices[0].position.x, vertices[0].position.z) - offsetX,
        glm::vec2(vertices[0].position.x, vertices[0].position.z) - offsetY - offsetX,
        x0,
        glm::vec2(x0.x, x0.y) - offsetY
    };
    frontRightInside->points = {
        x1,
        glm::vec2(x1.x, x1.y) - offsetY,
        glm::vec2(vertices[3].position.x, vertices[3].position.z) + offsetX,
        glm::vec2(vertices[3].position.x, vertices[3].position.z) - offsetY + offsetX
    };
    backInside->points = {
        glm::vec2(vertices[8].position.x, vertices[8].position.z) - offsetX,
        glm::vec2(vertices[8].position.x, vertices[8].position.z) - offsetX + offsetY,
        glm::vec2(vertices[11].position.x, vertices[11].position.z) + offsetX + offsetY,
        glm::vec2(vertices[11].position.x, vertices[11].position.z) + offsetX
    };
    leftInside->points = {
        frontLeftInside->points[0],
        glm::vec2(vertices[0].position.x, vertices[0].position.z),
        glm::vec2(vertices[8].position.x, vertices[8].position.z),
        backInside->points[0]
    };
    rightInside->points = {
        frontRightInside->points[2],
        glm::vec2(vertices[3].position.x, vertices[3].position.z),
        glm::vec2(vertices[11].position.x, vertices[11].position.z),
        backInside->points[3]
    };
}

void House::pre_draw()
{
    if (vertices.empty())
        generate_vertices();
    door.draw();
    cube.draw();
    cube2.draw();
    sphere.draw();
    leftSide.draw();
    rightSide.draw();
}

void House::set_position(const glm::vec3 position)
{
    this->position = position;
    collision->set_position(position);
    cube.set_position(position + rotateY(glm::vec3(3.2, 0.1, 0.7) * this->scale, -angle));
    cube2.set_position(position + rotateY(glm::vec3(2, 0.2, -0.5) * this->scale, -angle));
    sphere.set_position(position + rotateY(glm::vec3(0.7, 0.2, -.8) * this->scale, -angle));
    vertices.clear();
}

void House::set_rotation(const float angle)
{
    this->angle = angle;
    // this->collision->set_rotation(quat_cast(rot_y_mat(angle)));
    door.set_rotation(this->angle + doorAngle);
    cube.set_rotation(quat_cast(rot_y_mat(-angle)));
    cube2.set_rotation(quat_cast(rot_y_mat(-angle)));
    sphere.set_rotation(quat_cast(rot_y_mat(-angle)));
    vertices.clear();
}

void House::set_scale(const glm::vec3 scale)
{
    this->scale = scale;
    this->door.set_scale(scale.x);
    vertices.clear();
}

void House::set_door_offset(const float offset)
{
    doorOffset = offset;
    vertices.clear();
}

void House::set_door_rotation(const float angle)
{
    doorAngle = angle;
    door.set_rotation(this->angle + doorAngle);
}

std::vector<ICollision*> House::get_collisions()
{
    std::vector<ICollision*> collisions;
    collisions.reserve(9);
    collisions.push_back(doorOpen);
    collisions.push_back(collision);
    collisions.push_back(leftDoorGuard);
    collisions.push_back(rightDoorGuard);
    collisions.push_back(frontLeftInside);
    collisions.push_back(frontRightInside);
    collisions.push_back(backInside);
    collisions.push_back(leftInside);
    collisions.push_back(rightInside);
    return collisions;
}

bool House::is_inside() const
{
    return isInside;
}

void House::set_inside(const bool inside)
{
    isInside = inside;
    frontLeftInside->should_overlap = frontRightInside->should_overlap = backInside->should_overlap = leftInside->should_overlap = rightInside->should_overlap = !inside;
}