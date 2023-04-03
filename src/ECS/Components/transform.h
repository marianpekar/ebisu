#pragma once

#include "component.h"

class Transform : public Component
{
private:
	int x_pos, y_pos;
public:
	Transform() : x_pos(0), y_pos(0) {}
	virtual ~Transform() = default;
	const int& GetX() const { return x_pos; } 
	const int& GetY() const { return y_pos; }
	void SetX(int x) { x_pos = x; }
	void SetY(int y) { y_pos = y; }
	void SetPosition(int x, int y)
	{
		x_pos = x;
		y_pos = y;
	}
};