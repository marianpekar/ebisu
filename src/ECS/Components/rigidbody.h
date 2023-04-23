#pragma once

#include "../../Math/vector2.h"
#include "component.h"

class Rigidbody : public Component
{
private:
	class Transform* transform = nullptr;
	class MapCollider* map_collider = nullptr;
	Vector2 force;
	float mass, drag, acceleration;
public:
	Rigidbody() : force(0,0), mass(1), drag(0.01f), acceleration(1) {}
	void AddForce(Vector2& force) { this->force += force; }
	void Setup() override;
	void Update(float delta_time) override;
	void SetMass(const float& mass) { this->mass = mass; }
	void SetDrag(const float& drag) { this->drag = drag; }
};