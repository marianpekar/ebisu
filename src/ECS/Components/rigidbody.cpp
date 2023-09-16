#include "../entity.h"
#include "rigidbody.h"
#include "transform.h"
#include "map_collider.h"

void Rigidbody::Setup()
{
    transform = owner->GetComponent<Transform>();
    map_collider = owner->GetComponent<MapCollider>();
}

void Rigidbody::Update(float delta_time)
{
    const Vector2 current_pos = transform->GetPosition();
    const Vector2 prev_pos = current_pos;

    const Vector2 next_acceleration = force / mass;
    Vector2 target_pos = current_pos * 2.f - prev_pos + next_acceleration * (delta_time * delta_time);

    if (map_collider != nullptr)
    {
        map_collider->AdjustTargetPosition(current_pos, target_pos);
    }

    transform->SetPosition(target_pos);
    force *= drag;
}
void Rigidbody::SetMass(const float& new_mass) { this->mass = new_mass; }
void Rigidbody::SetDrag(const float& new_drag) { this->drag = 1.0f - new_drag; }