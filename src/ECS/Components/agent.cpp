#include "agent.h"
#include "rigidbody.h"
#include "transform.h"
#include "../entity.h"
#include "../../map.h"

void Agent::Setup()
{
    transform = owner->GetComponent<Transform>();
    rigidbody = owner->GetComponent<Rigidbody>();
}

void Agent::Update(float delta_time)
{
    current_path = map->FindPath(transform->GetPosition(), target_transform->GetPosition());

    if ((target_transform->GetPosition() - transform->GetPosition()).Length() <= stopping_distance ||
        current_path.size() < 2)
        return;

    const Vector2 direction = (current_path[1] - current_path[0]).Normalized();
    rigidbody->AddForce(direction * speed);
}

void Agent::SetTarget(Transform* target)
{
    target_transform = target;
}
