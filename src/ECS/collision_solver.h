#pragma once

#include <vector>

class CollisionSolver
{
private:
	std::vector<class BoxCollider*> colliders;
	std::vector<BoxCollider*> quad_result;
	bool AABB(BoxCollider* a, BoxCollider* b);
	class Quadtree* quad;
public:
	CollisionSolver();
	void AddCollider(BoxCollider* collider);
	void Update();
};