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
	if(on_collision == nullptr)
		return;

	on_collision(other);
}
