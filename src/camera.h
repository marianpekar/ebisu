#pragma once

#include "ECS/Components/transform.h"

class Camera
{
private:
	class Transform* target;
	int x, y;
	const int screen_half_width, screen_half_height;
public:
	Camera(Transform* target, int screen_width, int screen_height ) : 
		target(target), x(0), y(0), 
		screen_half_width(screen_width / 2), screen_half_height(screen_height / 2) {}
	const int& GetX() { return x; }
	const int& GetY() { return y; }
	void Update()
	{
		x = target->GetX() - screen_half_width;
		y = target->GetY() - screen_half_height;
	}
};