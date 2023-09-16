#pragma once

#include "../../Math/vector2.h"
#include "component.h"

class Transform final : public Component
{
private:
	Vector2 position, scale;
public:
	Transform() : position(), scale(Vector2(1,1)) {}
	~Transform() override = default;
	const Vector2& GetPosition() const { return position; } 
	const Vector2& GetScale() const { return scale; }
	void SetScale(const float& x, const float& y)
	{
		scale.x = x;
		scale.y = y;
	}
	void SetPosition(const float& x, const float& y)
	{
		position.x = x;
		position.y = y;
	}
	void SetPosition(const Vector2& new_position)
	{
		this->position.x = new_position.x;
		this->position.y = new_position.y;
	}
	void Move(const float& x, const float& y)
	{
		position.x += x;
		position.y += y;
	}
	void Move(const Vector2& dir)
	{
		position.x += dir.x;
		position.y += dir.y;
	}
	void Scale(const float& s) 
	{
		scale.x *= s;
		scale.y *= s;
	}
};