#pragma once

#include <unordered_map>
#include "component.h"
#include "../../Math/vector2.h"

class BoxCollider : public Component
{
private:
	Vector2 position;
	float width, height;
	bool is_trigger = false;
	class Transform* transform = nullptr;
	class CollisionSolver* collision_solver;
	std::unordered_map<class BoxCollider*, bool> others;
public:
	BoxCollider(float width, float height, CollisionSolver* collision_solver);
	void Setup() override;
	void Update(float delta_time) override;
	const bool& GetIsTrigger() { return is_trigger; }
	const float& GetX() { return position.x; }
	const float& GetY() { return position.y; }
	const float& GetWidth() { return width; }
	const float& GetHeight() { return height; }
	void AddOther(BoxCollider* other) { others[other] = true; }
	void RemoveOther(BoxCollider* other) { others.erase(other); }
	const std::unordered_map<class BoxCollider*, bool>& GetOthers() { return others; }

};