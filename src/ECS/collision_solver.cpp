#include "collision_solver.h"
#include "Components/box_collider.h"

CollisionSolver::CollisionSolver() {}

void CollisionSolver::AddCollider(BoxCollider* collider)
{
	colliders.emplace_back(collider);
}

void CollisionSolver::Update()
{
	for (size_t i = 0; i < colliders.size(); ++i)
	{
		for (size_t j = 0; j < colliders.size(); ++j)
		{
			auto& a = colliders[j];
			auto& b = colliders[i];

			if (a == b) 
				continue;
			
			bool intersects = AABB(a, b);
			if (intersects)
			{
				a->AddOther(b);
			}
			else
			{
				a->RemoveOther(b);
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