#include "collision_solver.h"
#include "../quadtree.h"
#include "Components/box_collider.h"
#include "Components/transform.h"
#include "entity.h"

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
			BoxCollider* a = colliders[i];
			BoxCollider* b = quad_result[j];

			if (a == b)
				continue;

			Vector2 overlap;
			bool intersects = SAT(a, b, overlap);
			if (intersects)
			{
				collisions.emplace_back(a, b, overlap);
			}
		}
	}

	for (auto& col : collisions)
	{
		col.a->Collide(col.b, col.overlap);
		col.b->Collide(col.a, -col.overlap);
	}

	collisions.clear();
}

bool CollisionSolver::SAT(BoxCollider* a, BoxCollider* b, Vector2& overlap)
{
	Vector2 a_center = a->GetPosition() + Vector2(a->GetHalfWidth(), a->GetHalfHeight());
	Vector2 b_center = b->GetPosition() + Vector2(b->GetHalfWidth(), b->GetHalfHeight());

	float x_dist = abs(a_center.x - b_center.x);
	float y_dist = abs(a_center.y - b_center.y);

	float x_min_dist = a->GetHalfWidth() + b->GetHalfWidth();
	float y_min_dist = a->GetHalfHeight() + b->GetHalfHeight();

	if (x_dist < x_min_dist && y_dist < y_min_dist) {
		float x_overlap = x_min_dist - x_dist;
		float y_overlap = y_min_dist - y_dist;

		if (x_overlap < y_overlap) {
			if (a_center.x < b_center.x) {
				overlap = Vector2(-x_overlap, 0.0f);
			}
			else {
				overlap = Vector2(x_overlap, 0.0f);
			}
		}
		else {
			if (a_center.y < b_center.y) {
				overlap = Vector2(0.0f, -y_overlap);
			}
			else {
				overlap = Vector2(0.0f, y_overlap);
			}
		}

		return true;
	}

	return false;
}

