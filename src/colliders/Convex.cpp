#include "Convex.h"
#include "../objects/Mesh.h"
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>

ConvexHull::ConvexHull()
{
}

ConvexHull::ConvexHull(Mesh *mesh)
{
    this->mesh = mesh;
    auto xz = glm::vec3(1, 0, 1);
    auto y = glm::vec3(0, 1, 0);

    // Calculate the convex hull with Graham's scan algorithm

    // Find the point with the lowest y-coordinate
    auto lowest = 0;
    for (auto i = 1; i < mesh->vertices.size(); i++)
    {
        if (mesh->vertices[i].position.y < mesh->vertices[lowest].position.y)
        {
            lowest = i;
        }
    }

    // Sort the points in a copy by the angle they and the lowest point make with the x-axis
    auto lowestPoint = mesh->vertices[lowest].position;
    auto sorted = mesh->vertices;
    std::sort(sorted.begin(), sorted.end(), [lowestPoint](const Vertex &a, const Vertex &b)
              {
        auto angleA = glm::angle(glm::normalize(a.position - lowestPoint), glm::vec3(1, 0, 0));
        auto angleB = glm::angle(glm::normalize(b.position - lowestPoint), glm::vec3(1, 0, 0));
        return angleA < angleB; });

    // Initialize the stack with the lowest point in the x axis
    std::vector<unsigned> stack;
    stack.push_back(lowest);

    // Iterate over the sorted points
    for (auto i = 1; i < sorted.size(); i++)
    {
        // If the stack has less than 2 points, add the next point
        if (stack.size() < 2)
        {
            stack.push_back(i);
            continue;
        }

        // Calculate the cross product of the last two points and the next point
        auto a = mesh->vertices[stack[stack.size() - 2]].position;
        auto b = mesh->vertices[stack[stack.size() - 1]].position;
        auto c = mesh->vertices[i].position;
        auto cross = glm::cross(b - a, c - b);

        // If the cross product is negative, pop the last point from the stack
        if (cross.y < 0)
        {
            stack.pop_back();
            i--;
        }
        else
        {
            stack.push_back(i);
        }
    }

    // Copy the stack to the indices vector
    indices = stack;
}

bool ConvexHull::intersects(const ConvexHull &other) const
{
    // Check if the bounding boxes of the two convex hulls intersect with use of the separating axis theorem
    return false;
}

void ConvexHull::rebound(const ConvexHull &other, glm::vec3 &direction) const
{
    direction = glm::vec3(0.0f);
}