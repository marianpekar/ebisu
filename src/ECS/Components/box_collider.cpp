#include "../collision_solver.h"
#include "../entity.h"
#include "box_collider.h"
#include "transform.h"

BoxCollider::BoxCollider(float width, float height, CollisionSolver* collision_solver) : 
	position(Vector2(0, 0)), width(width), height(height), collision_solver(collision_solver) {}

void BoxCollider::Setup()
{
	transform = owner->GetComponent<Transform>();
	collision_solver->AddCollider(this);
}

void BoxCollider::Update(float delta_time)
{
	position = transform->GetPosition();
}

void BoxCollider::Collide(BoxCollider* other)
{
	if (!is_trigger)
	{
		Transform* other_transform = other->owner->GetComponent<Transform>();

		Vector2 dir = other_transform->GetPosition() - transform->GetPosition();

		float len = dir.Length();
		if (len > 0) {
			float inv_len = 1.0f / len;
			dir *= inv_len;
		}

		float push_back_dist = -1.f;
		transform->Move(dir * push_back_dist);
		other->transform->Move(dir * -push_back_dist);	
	}

	if(on_collision == nullptr)
		return;

	on_collision(other);
}
