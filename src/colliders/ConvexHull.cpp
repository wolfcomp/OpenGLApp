#include "ConvexHull.h"
#include "../objects/Mesh.h"
#include <algorithm>
#include <glm/gtx/vector_angle.hpp>

// Jarvis_march algorithm based on https://www.sanfoundry.com/cpp-program-implement-gift-wrapping-algorithm-two-dimensions/

int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r)
{
    auto val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
}

std::vector<unsigned> jarvis_march(std::vector<glm::vec2> points)
{
    std::vector<unsigned> stack;

    if (points.size() < 3)
        return stack;

    unsigned n = points.size();
    unsigned l = 0;

    for (auto i = 1; i < n; i++)
    {
        if (points[i].x < points[l].x)
        {
            l = i;
        }
    }
    stack.push_back(l);

    unsigned p = l, q;
    do
    {
        q = (p + 1) % n;
        for (auto i = 0; i < n; i++)
        {
            if (orientation(points[p], points[i], points[q]) == 2)
            {
                q = i;
            }
        }
        stack.push_back(q);
        p = q;
    } while (p != l);

    stack.pop_back();

    return stack;
}

ConvexHull::ConvexHull()
{
}

ConvexHull::ConvexHull(Mesh *mesh)
{
    this->mesh = mesh;

    // Find the point with the lowest and highest y-coordinate
    auto lowest = 0;
    auto highest = 0;
    for (auto i = 1; i < mesh->vertices.size(); i++)
    {
        if (mesh->vertices[i].position.y < mesh->vertices[lowest].position.y)
        {
            lowest = i;
        }
        if (mesh->vertices[i].position.y > mesh->vertices[highest].position.y)
        {
            highest = i;
        }
    }

    // Sort the points in a copy by the angle they and the lowest point make with the x-axis
    auto lowestPos = mesh->vertices[lowest].position;
    auto highestPos = mesh->vertices[highest].position;
    auto sorted = std::vector<glm::vec2>();
    auto sortedSize = 0;
    for (auto vert : mesh->vertices)
    {
        auto p = vert.position.xz();
        if (find(sorted.begin(), sorted.end(), p) == sorted.end())
        {
            sortedSize++;
            sorted.push_back(p);
        }
    }

    // Initialize the stack with the lowest point in the x axis
    auto stack = jarvis_march(sorted);

    // Create the convex hull from the stack indices of the positions in sorted vector to construct a new mesh
    for (auto i = 0; i < stack.size(); i++)
    {
        auto pos00 = sorted[stack[i]];
        auto pos01 = sorted[stack[(i + 1) % stack.size()]];
        auto pos10 = glm::vec3(pos00.x, highestPos.y, pos00.y);
        auto pos11 = glm::vec3(pos01.x, highestPos.y, pos01.y);
        vertices.push_back(Vertex(glm::vec3(pos00.x, lowestPos.y, pos00.y)));
        vertices.push_back(Vertex(glm::vec3(pos01.x, lowestPos.y, pos01.y)));
        vertices.push_back(Vertex(pos10));
        vertices.push_back(Vertex(pos11));
        indices.push_back(i * 4);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 2);
        indices.push_back(i * 4 + 1);
        indices.push_back(i * 4 + 3);
        indices.push_back(i * 4 + 2);
    }
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

std::vector<glm::vec3> ConvexHull::get_points() const
{
    return std::vector<glm::vec3>();
}