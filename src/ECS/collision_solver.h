#pragma once

#include <memory>
#include <vector>
#include "../Math/vector2.h"

class CollisionSolver
{
private:
    std::vector<std::shared_ptr<class Collider>> colliders;
    std::vector<std::shared_ptr<Collider>> quad_result;
    static bool SAT(const std::shared_ptr<Collider>& a, const std::shared_ptr<Collider>& b, Vector2& overlap);
    std::shared_ptr<class Quadtree> quad;
public:
    CollisionSolver(float quad_x, float quad_y, float quad_width, float quad_height);
    void AddCollider(const std::shared_ptr<Collider>& collider);
    void Update();
    void Clear();
};
