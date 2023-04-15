#pragma once

#include <vector>

class CollisionSolver
{
private:
	std::vector<class BoxCollider*> colliders;
	bool AABB(BoxCollider* a, BoxCollider* b);
public:
	void AddCollider(BoxCollider* collider);
	void Update();
};