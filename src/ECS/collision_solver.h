#pragma once

#include <vector>
#include "../Math/vector2.h"

class CollisionSolver
{
private:
	struct Collision
	{
		class BoxCollider* a;
		class BoxCollider* b;
		Vector2 overlap;
	};

	std::vector<BoxCollider*> colliders;
	std::vector<BoxCollider*> quad_result;
	std::vector<Collision> collisions;
	bool SAT(BoxCollider* a, BoxCollider* b, Vector2& overlap);
	class Quadtree* quad;
public:
	CollisionSolver(float quad_x, float quad_y, float quad_width, float quad_height);
	void AddCollider(BoxCollider* collider);
	void Update();
};