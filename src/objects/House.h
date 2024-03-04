#pragma once
#include "Door.h"
#include "../primitives/Sphere.h"
#include "../primitives/Cube.h"
#include "../interfaces/ITriangleObject.h"
#include "../primitives/Cube.h"

struct OBB;

class House final : public ITriangleObject
{
    glm::vec3 position;
    float angle;
    float doorAngle;
    glm::vec3 scale;
    float doorOffset;
    void generate_vertices();
    Door door;
    Sphere sphere;
    Cube cube;
    Cube cube2;
    Cube leftSide;
    Cube rightSide;
    OBB* doorOpen;
    OBB* leftDoorGuard;
    OBB* rightDoorGuard;
    OBB* backInside;
    OBB* leftInside;
    OBB* rightInside;
    OBB* frontLeftInside;
    OBB* frontRightInside;
    bool isInside;

public:
    std::vector<Vertex> ProperVertexes;
    std::vector<Vertex> OffsetVertexes;
    House();
    void set_position(const glm::vec3 position);
    void set_scale(const glm::vec3 scale);
    void set_rotation(const float angle);
    void set_door_offset(const float offset);
    void set_door_rotation(const float angle);
    bool is_inside() const;
    void set_inside(const bool inside);
    void pre_draw() override;
    std::vector<ICollision*> get_collisions() override;
};
