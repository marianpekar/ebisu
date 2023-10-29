#include "agent.h"
#include "../entity.h"
#include "transform.h"
#include "../../map.h"

void Agent::Setup()
{
    transform = owner->GetComponent<Transform>();
}

void Agent::Update(float delta_time)
{
    current_path = map->FindPath(transform->GetPosition(), target_transform->GetPosition());
}

void Agent::SetTarget(Transform* target)
{
    target_transform = target;
}
