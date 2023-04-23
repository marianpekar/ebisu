#include "../collision_solver.h"
#include "../entity.h"
#include "box_collider.h"
#include "transform.h"
#include "rigidbody.h"

BoxCollider::BoxCollider(float width, float height, CollisionSolver* collision_solver) : 
	position(Vector2(0, 0)), width(width), height(height), collision_solver(collision_solver), resistance(32) {}

void BoxCollider::Setup()
{
	transform = owner->GetComponent<Transform>();
	rigidbody = owner->GetComponent<Rigidbody>();
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
		Vector2 push_force = (other_transform->GetPosition() - transform->GetPosition()) * -resistance;

		rigidbody->AddForce(push_force);
	}

	if(on_collision == nullptr)
		return;

	on_collision(other);
}
