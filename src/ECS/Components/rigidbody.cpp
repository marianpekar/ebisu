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
    Vector2 current_pos = transform->GetPosition();
    Vector2 prev_pos = current_pos;

    Vector2 acceleration = force / mass;
    Vector2 target_pos = current_pos * 2.f - prev_pos + acceleration * (delta_time * delta_time);

    if (map_collider != nullptr)
    {
        map_collider->AdjustTargetPosition(current_pos, target_pos);
    }

    transform->SetPosition(target_pos);
    force *= drag;
}