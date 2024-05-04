#include "Collider.h"
#include <iostream>

bool GJK::same_direction(glm::vec3 direction, glm::vec3 ao)
{
    return dot(direction, ao) > 0;
}

bool GJK::line(Simplex &simplex, glm::vec3 &direction)
{
    auto a = simplex[0];
    auto b = simplex[1];
    auto ab = b - a;
    auto ao = -a;
    if (same_direction(ab, ao))
        direction = cross(cross(ab, ao), ab);
    else
    {
        simplex = {a};
        direction = ao;
    }

    return false;
}

bool GJK::triangle(Simplex &simplex, glm::vec3 &direction)
{
    auto a = simplex[0];
    auto b = simplex[1];
    auto c = simplex[2];

    auto ab = b - a;
    auto ac = c - a;
    auto ao = -a;

    auto abc = cross(ab, ac);

    if (same_direction(cross(abc, ac), ao))
    {
        if (same_direction(ac, ao))
        {
            simplex = {a, c};
            direction = cross(cross(ac, ao), ac);
        }
        else
            return line(simplex = {a, b}, direction);
    }
    else
    {
        if (same_direction(cross(ab, abc), ao))
            return line(simplex = {a, b}, direction);
        else
        {
            if (same_direction(abc, ao))
                direction = abc;
            else
            {
                simplex = {a, c, b};
                direction = -abc;
            }
        }
    }
    return false;
}

bool GJK::tetrahedron(Simplex &simplex, glm::vec3 &direction)
{
    auto a = simplex[0];
    auto b = simplex[1];
    auto c = simplex[2];
    auto d = simplex[3];

    auto ab = b - a;
    auto ac = c - a;
    auto ad = d - a;
    auto ao = -a;

    auto abc = cross(ab, ac);
    auto acd = cross(ac, ad);
    auto adb = cross(ad, ab);

    if (same_direction(abc, ao))
        return triangle(simplex = {a, b, c}, direction);
    else if (same_direction(acd, ao))
        return triangle(simplex = {a, c, d}, direction);
    else if (same_direction(adb, ao))
        return triangle(simplex = {a, d, b}, direction);
    else
        return true;
}

bool GJK::next_simplex(Simplex &simplex, glm::vec3 &direction)
{
    switch (simplex.size())
    {
    case 2:
        return line(simplex, direction);
    case 3:
        return triangle(simplex, direction);
    case 4:
        return tetrahedron(simplex, direction);
    }

    return false; // if this is ever hit something has gone wrong
}

bool GJK::gjk(const ColliderBase &a, const ColliderBase &b)
{
    auto support = a.support(b, glm::vec3(1, 0, 0));

    Simplex simplex;
    simplex.push_front(support);

    auto direction = -support;

    while (true)
    {
        support = a.support(b, direction);

        if (dot(support, direction) <= 0)
            return false; // no collision

        simplex.push_front(support);

        if (next_simplex(simplex, direction))
            return true;
    }
}

void debug_collider_log(ColliderHandler *a, ColliderHandler *b)
{
    std::cout << "Collision between " << a << " and " << b << std::endl;
}

ColliderBase::ColliderBase()
{
    handler = new ColliderHandler(this);
    handler->callback = &debug_collider_log;
}