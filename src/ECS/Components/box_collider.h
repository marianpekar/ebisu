#pragma once

#include "component.h"

class BoxCollider : public Component
{
private:
	float x, y, width, height;
	bool is_trigger = false;
	class Transform* transform = nullptr;
	class CollisionSolver* collision_solver;
	bool has_collision = false;
	BoxCollider* other = nullptr;
public:
	BoxCollider(float width, float height, CollisionSolver* collision_solver);
	void Setup() override;
	void Update(float delta_time) override;
	const bool& GetIsTrigger() { return is_trigger; }
	const float& GetX() { return x; }
	const float& GetY() { return y; }
	const float& GetWidth() { return width; }
	const float& GetHeight() { return height; }
	const bool& GetHasCollision() { return has_collision; }
	void SetHasCollision(bool value) { has_collision = value; }
	void SetOther(BoxCollider* other) { this->other = other; }
	BoxCollider* GetOther() { return other; }
	void ClearOther() { other = nullptr; }

};