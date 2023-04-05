#pragma once

#include "component.h"

class Transform : public Component
{
private:
	float x_pos, y_pos;
public:
	Transform() : x_pos(0), y_pos(0) {}
	virtual ~Transform() = default;
	const float& GetX() const { return x_pos; } 
	const float& GetY() const { return y_pos; }
	void SetX(const float& x) { x_pos = x; }
	void SetY(const float& y) { y_pos = y; }
	void MoveX(const float& x) { x_pos += x; }
	void MoveY(const float& y) { y_pos += y; }
	void SetPosition(const float& x, const float& y)
	{
		x_pos = x;
		y_pos = y;
	}
	void Move(const float& x, const float& y)
	{
		x_pos += x;
		y_pos += y;
	}
};