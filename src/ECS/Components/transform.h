#pragma once

#include "component.h"

class Transform : public Component
{
private:
	float x_pos, y_pos, x_scale, y_scale;
public:
	Transform() : x_pos(0), y_pos(0), x_scale(1), y_scale(1) {}
	virtual ~Transform() = default;
	const float& GetX() const { return x_pos; } 
	const float& GetY() const { return y_pos; }
	const float& GetXScale() const { return x_scale; }
	const float& GetYScale() const { return y_scale; }
	void SetX(const float& x) { x_pos = x; }
	void SetY(const float& y) { y_pos = y; }
	void SetXScale(const float& s) { x_scale = s; }
	void SetYScale(const float& s) { y_scale = s; }
	void MoveX(const float& x) { x_pos += x; }
	void MoveY(const float& y) { y_pos += y; }
	void ScaleX(const float& s) { x_scale *= s; }
	void ScaleY(const float& s) { y_scale *= s; }
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
	void Scale(const float& s) 
	{
		x_scale *= s;
		y_scale *= s;
	}
};