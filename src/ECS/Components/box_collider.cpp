#include "../collision_solver.h"
#include "../entity.h"
#include "box_collider.h"
#include "transform.h"
#include "map_collider.h"
#include "rigidbody.h"

BoxCollider::BoxCollider(const float width, const float height, CollisionSolver* collision_solver) : 
    position(Vector2(0, 0)), width(width), height(height), half_width(width * 0.5f), half_height(height * 0.5f),
    collision_solver(collision_solver) {}

void BoxCollider::Setup()
{
    transform = owner->GetComponent<Transform>();
    map_collider = owner->GetComponent<MapCollider>();
    rigidbody = owner->GetComponent<Rigidbody>();
    collision_solver->AddCollider(this);
}

void BoxCollider::Update(float delta_time)
{
    position = transform->GetPosition();
}

void BoxCollider::Collide(BoxCollider* other, const Vector2& overlap) const
{
    if (!is_trigger)
    {
        const Vector2 current_pos = transform->GetPosition();
        Vector2 target_pos = current_pos + overlap / 2.0f;

        if (map_collider != nullptr)
        {
            map_collider->AdjustTargetPosition(current_pos, target_pos);
        }
        transform->SetPosition(target_pos);

        if (rigidbody != nullptr && other->rigidbody != nullptr)
        {
            Vector2 force = rigidbody->GetForce();
            other->rigidbody->AddForce(force);

            Vector2 opposite_force = -force;
            rigidbody->AddForce(opposite_force);
        }
    }

    if(on_collision == nullptr)
        return;

    on_collision(other);
}
