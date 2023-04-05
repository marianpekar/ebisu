#pragma once

#include "component.h"

class PlayerController : public Component
{
private:
	bool& is_game_running;
	float move_speed = 300.0f;
	float x, y;
	class SpriteSheet* sprite_sheet = nullptr;
	class Transform* transform = nullptr;
public:
	PlayerController(bool& is_game_running) : is_game_running(is_game_running), x(0), y(0) {}
	virtual ~PlayerController() = default;
	void Setup() override;
	void Update(float delta_time) override;
	void SetMoveSpeed(float move_speed) { this->move_speed = move_speed; }
};