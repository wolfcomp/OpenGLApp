#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <array>
#include <map>
#include <initializer_list>
#include <glm/geometric.hpp>

struct ColliderBase;
struct ColliderHandler;
struct Mesh;

struct Simplex
{
private:
    std::array<glm::vec3, 4> points;
    int m_size;

public:
    Simplex() : m_size(0) {}

    Simplex &operator=(std::initializer_list<glm::vec3> list)
    {
        m_size = 0;
        for (auto p : list)
            points[m_size++] = p;

        return *this;
    }

    void push_front(glm::vec3 point)
    {
        points = {point, points[0], points[1], points[2]};
        m_size = std::min(m_size + 1, 4);
    }

    glm::vec3 &operator[](int i) { return points[i]; }
    size_t size() const { return m_size; }

    auto begin() const { return points.begin(); }
    auto end() const { return points.end() - (4 - m_size); }
};

struct GJK
{
    static bool line(Simplex &simplex, glm::vec3 &direction);
    static bool triangle(Simplex &simplex, glm::vec3 &direction);
    static bool tetrahedron(Simplex &simplex, glm::vec3 &direction);
    static bool next_simplex(Simplex &simplex, glm::vec3 &direction);
    static bool same_direction(glm::vec3 a, glm::vec3 b);
    static bool gjk(const ColliderBase &a, const ColliderBase &b);
};

struct ColliderBase
{
    GJK gjk;
    ColliderHandler *handler;
    Mesh *parent;

    ColliderBase(Mesh *parent);

    virtual ~ColliderBase() = default;

    virtual bool intersects(const ColliderBase &other) const = 0;

    virtual void rebound(const ColliderBase &other, glm::vec3 &direction) const = 0;

    virtual std::vector<glm::vec3> get_points() const = 0;

    void resolve_collision(const ColliderBase &other) const;

    glm::vec3 find_furthest_point(glm::vec3 direction) const;

    glm::vec3 support(const ColliderBase &other, glm::vec3 direction) const
    {
        return this->find_furthest_point(direction) - other.find_furthest_point(-direction);
    }
};

template <typename T>
struct Collider : public ColliderBase
{
    Collider(Mesh *parent) : ColliderBase(parent) {}

    virtual bool intersects(const T &other) const = 0;

    virtual void rebound(const T &other, glm::vec3 &direction) const = 0;

    bool intersects(const ColliderBase &other) const override
    {
        if (dynamic_cast<const T *>(&other))
            return intersects(*dynamic_cast<const T *>(&other));
        return gjk.gjk(*this, other);
    }

    void rebound(const ColliderBase &other, glm::vec3 &direction) const override
    {
        if (dynamic_cast<const T *>(&other))
            return rebound(*dynamic_cast<const T *>(&other), direction);

        // TODO: Implement rebound for other types of colliders

        direction = glm::vec3(0.0f);
    }
};

struct ColliderHandler
{
    enum Type
    {
        NONE,
        OVERLAP,
        COLLIDE
    };

    enum ObjectType
    {
        PLAYER,
        ENEMY,
        PAWN,
        WORLD_DYNAMIC,
        WORLD_STATIC
    } objectType = ObjectType::WORLD_STATIC;

    std::map<ObjectType, Type> types = {{ObjectType::PLAYER, Type::COLLIDE},
                                        {ObjectType::ENEMY, Type::COLLIDE},
                                        {ObjectType::PAWN, Type::OVERLAP},
                                        {ObjectType::WORLD_DYNAMIC, Type::COLLIDE},
                                        {ObjectType::WORLD_STATIC, Type::COLLIDE}};
    ColliderBase *collider;
    bool active = true;

    ColliderHandler() : collider(nullptr) {}
    ColliderHandler(ColliderBase *collider) : collider(collider) {}

    /// @brief Checks if the collider is colliding with another collider
    /// @param other The other collider to check against
    /// @return True if the colliders are colliding and should block further execution, false otherwise
    Type check(const ColliderHandler &other) const;
};