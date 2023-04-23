#pragma once

#include <vector>
#include <utility>

class CollisionSolver
{
private:
	std::vector<class BoxCollider*> colliders;
	std::vector<BoxCollider*> quad_result;
	std::vector<std::pair<BoxCollider*, BoxCollider*>> collisions;
	bool AABB(BoxCollider* a, BoxCollider* b);
	class Quadtree* quad;
public:
	CollisionSolver(float quad_x, float quad_y, float quad_width, float quad_height);
	void AddCollider(BoxCollider* collider);
	void Update();
};