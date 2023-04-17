#pragma once

#include "component.h"
#include <unordered_map>

class BoxCollider : public Component
{
private:
	float x, y, width, height;
	bool is_trigger = false;
	class Transform* transform = nullptr;
	class CollisionSolver* collision_solver;
	std::unordered_map<class BoxCollider*, bool> others;
public:
	BoxCollider(float width, float height, CollisionSolver* collision_solver);
	void Setup() override;
	void Update(float delta_time) override;
	const bool& GetIsTrigger() { return is_trigger; }
	const float& GetX() { return x; }
	const float& GetY() { return y; }
	const float& GetWidth() { return width; }
	const float& GetHeight() { return height; }
	void AddOther(BoxCollider* other) { others[other] = true; }
	void RemoveOther(BoxCollider* other) { others.erase(other); }
	const std::unordered_map<class BoxCollider*, bool>& GetOthers() { return others; }

};