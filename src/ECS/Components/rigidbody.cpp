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

    Vector2 acceleration = (force / mass) * (1.f - drag);
    Vector2 target_pos = current_pos * 2.f - prev_pos + acceleration * (delta_time * delta_time);

    if (map_collider != nullptr)
    {
        if (map_collider->HasCollisionAt(Vector2(target_pos.x, current_pos.y)))
        {
            target_pos.x = current_pos.x;
        }

        if (map_collider->HasCollisionAt(Vector2(current_pos.x, target_pos.y)))
        {
            target_pos.y = current_pos.y;
        }
    }

    transform->SetPosition(target_pos);
    force = Vector2(0, 0);
}