#include "agent.h"

#include <__msvc_filebuf.hpp>

#include "rigidbody.h"
#include "transform.h"
#include "../entity.h"
#include "../../map.h"
#include "../../path.h"

void Agent::Setup()
{
    transform = owner->GetComponent<Transform>();
    rigidbody = owner->GetComponent<Rigidbody>();
}

void Agent::Update(float delta_time)
{
    const std::vector<Vector2> current_path = map->FindPath(transform->GetPosition(), target_transform->GetPosition());
    const auto smooth_path = new Path(current_path, transform->GetPosition(), 5.0f);
    
    if ((target_transform->GetPosition() - transform->GetPosition()).Length() <= stopping_distance ||
        current_path.size() < 2)
        return;

    const Vector2 direction = (smooth_path->path[1] - smooth_path->path[0]).Normalized();
    rigidbody->AddForce(direction * speed);

    delete smooth_path;
}

void Agent::SetTarget(Transform* target)
{
    target_transform = target;
}
