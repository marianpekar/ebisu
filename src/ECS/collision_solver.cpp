#include "collision_solver.h"
#include "Components/box_collider.h"

void CollisionSolver::AddCollider(BoxCollider* collider)
{
	colliders.emplace_back(collider);
}

void CollisionSolver::Update()
{
	for (auto& a : colliders)
	{
		for (auto& b : colliders)
		{
			bool intersects = AABB(a, b);
			a->SetHasCollision(intersects);
			b->SetHasCollision(intersects);

			if (intersects)
			{
				a->SetOther(b);
				b->SetOther(a);
			}
			else
			{
				a->ClearOther();
				b->ClearOther();
			}
		}
	}
}

bool CollisionSolver::AABB(BoxCollider* a, BoxCollider* b)
{
	if (a->GetX() + a->GetWidth() < b->GetX() || a->GetX() > b->GetX() + b->GetWidth())
	{
		return false;
	}

	if (a->GetY() + a->GetHeight() < b->GetY() || a->GetY() > b->GetY() + b->GetHeight())
	{
		return false;
	}

	return true;
}