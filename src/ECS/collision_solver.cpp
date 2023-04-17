#include "collision_solver.h"
#include "quadtree.h"
#include "Components/box_collider.h"

CollisionSolver::CollisionSolver(float quad_x, float quad_y, float quad_width, float quad_height) :
	quad(new Quadtree(0, quad_x, quad_y, quad_width, quad_height)) {}

void CollisionSolver::AddCollider(BoxCollider* collider)
{
	colliders.emplace_back(collider);
}

void CollisionSolver::Update()
{
	quad->Clear();
	for (size_t i = 0; i < colliders.size(); ++i)
	{
		quad->Insert(colliders[i]);
	}

	for (size_t i = 0; i < colliders.size(); ++i)
	{
		quad_result.clear();
		quad->Retrieve(quad_result, colliders[i]);

		for (size_t j = 0; j < quad_result.size(); ++j)
		{
			auto& a = colliders[i];
			auto& b = quad_result[j];

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