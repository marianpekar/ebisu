#pragma once

#include <memory>
#include <vector>
#include "../Math/vector2.h"

class CollisionSolver
{
private:
    struct Collision
    {
        class BoxCollider* a;
        BoxCollider* b;
        Vector2 overlap;
    };

    std::vector<std::shared_ptr<BoxCollider>> colliders;
    std::vector<std::shared_ptr<BoxCollider>> quad_result;
    std::vector<Collision> collisions;
    static bool SAT(const std::shared_ptr<BoxCollider>& a, const std::shared_ptr<BoxCollider>& b, Vector2& overlap);
    class Quadtree* quad;
public:
    CollisionSolver(float quad_x, float quad_y, float quad_width, float quad_height);
    void AddCollider(const std::shared_ptr<BoxCollider>& collider);
    void Update();
    void Clear();
};