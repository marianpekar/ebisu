#include "../entity.h"
#include "box_collider.h"
#include "transform.h"

void BoxCollider::Setup()
{
	transform = owner->GetComponent<Transform>();
}

void BoxCollider::Update(float delta_time)
{
	x = transform->GetX();
	y = transform->GetY();
}