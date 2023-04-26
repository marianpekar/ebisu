#pragma once

#include "component.h"
#include <functional>
#include "../../Math/vector2.h"

class BoxCollider : public Component
{
private:
	Vector2 position;
	float width, height;
	float half_width, half_height;
	bool is_trigger = false;
	class Transform* transform = nullptr;
	class MapCollider* map_collider = nullptr;
	class Rigidbody* rigidbody = nullptr;
	class CollisionSolver* collision_solver;
public:
	BoxCollider(float width, float height, CollisionSolver* collision_solver);
	void Setup() override;
	void Update(float delta_time) override;
	std::function<void(BoxCollider*)> on_collision;
	void Collide(BoxCollider* other, const Vector2& overlap);
	const bool& GetIsTrigger() { return is_trigger; }
	void SetIsTrigger(const bool& value) { is_trigger = value; }
	const float& GetX() { return position.x; }
	const float& GetY() { return position.y; }
	const float& GetWidth() { return width; }
	const float& GetHeight() { return height; }
	const float& GetHalfWidth() { return half_width; }
	const float& GetHalfHeight() { return half_height; }
	const Vector2& GetPosition() { return position; }
};