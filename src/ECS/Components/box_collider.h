#pragma once

#include "component.h"
#include <functional>
#include "../../Math/vector2.h"

class BoxCollider : public Component
{
private:
	Vector2 position;
	float width, height;
	bool is_trigger = false;
	class Transform* transform = nullptr;
	class CollisionSolver* collision_solver;
public:
	BoxCollider(float width, float height, CollisionSolver* collision_solver);
	void Setup() override;
	void Update(float delta_time) override;
	std::function<void(BoxCollider*)> on_collision;
	void Collide(BoxCollider* other);
	const bool& GetIsTrigger() { return is_trigger; }
	const float& GetX() { return position.x; }
	const float& GetY() { return position.y; }
	const float& GetWidth() { return width; }
	const float& GetHeight() { return height; }
};