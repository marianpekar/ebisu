#include "collision_solver.h"
#include "../quadtree.h"
#include "Components/box_collider.h"
#include "entity.h"

CollisionSolver::CollisionSolver(const float quad_x, const float quad_y, const float quad_width, const float quad_height) :
    quad(new Quadtree(0, quad_x, quad_y, quad_width, quad_height)) {}

void CollisionSolver::AddCollider(BoxCollider* collider)
{
    colliders.emplace_back(collider);
}

void CollisionSolver::Update()
{
    quad->Clear();
    for (const auto& collider : colliders)
    {
        quad->Insert(collider);
    }

    for (const auto& collider : colliders)
    {
        quad_result.clear();
        quad->Retrieve(quad_result, collider);

        for (auto b : quad_result)
        {
            BoxCollider* a = collider;
            if (a == b)
                continue;

            if (Vector2 overlap; SAT(a, b, overlap))
            {
                collisions.emplace_back(a, b, overlap);
            }
        }
    }

    for (auto& [a, b, overlap] : collisions)
    {
        a->Collide(b, overlap);
        b->Collide(a, -overlap);
    }

    collisions.clear();
}

bool CollisionSolver::SAT(const BoxCollider* a, const BoxCollider* b, Vector2& overlap)
{
    const Vector2 a_center = a->GetPosition() + Vector2(a->GetHalfWidth(), a->GetHalfHeight());
    const Vector2 b_center = b->GetPosition() + Vector2(b->GetHalfWidth(), b->GetHalfHeight());

    const float x_dist = abs(a_center.x - b_center.x);
    const float y_dist = abs(a_center.y - b_center.y);

    const float x_min_dist = a->GetHalfWidth() + b->GetHalfWidth();
    const float y_min_dist = a->GetHalfHeight() + b->GetHalfHeight();

    if (x_dist < x_min_dist && y_dist < y_min_dist)
    {
        const float x_overlap = x_min_dist - x_dist;
        const float y_overlap = y_min_dist - y_dist;
        
        if (x_overlap < y_overlap)
        {
            if (a_center.x < b_center.x)
            {
                overlap = Vector2(-x_overlap, 0.0f);
            }
            else
            {
                overlap = Vector2(x_overlap, 0.0f);
            }
        }
        else
        {
            if (a_center.y < b_center.y)
            {
                overlap = Vector2(0.0f, -y_overlap);
            }
            else
            {
                overlap = Vector2(0.0f, y_overlap);
            }
        }

        return true;
    }

    return false;
}