#pragma once

#include "component.h"

class BoxCollider : public Component
{
private:
	float x, y, width, height;
	bool is_trigger = false;
	class Transform* transform = nullptr;
public:
	BoxCollider(float width, float height) : x(0), y(0), width(width), height(height) {}
	void Setup() override;
	void Update(float delta_time) override;
	const bool& GetIsTrigger() { return is_trigger; }
};