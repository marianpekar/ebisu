#pragma once

#include "component.h"

class PlayerController : public Component
{
private:
	bool& is_game_running;
	float move_speed = 300.0f;
	float x, y;
	class Animator* animator = nullptr;
	class Transform* transform = nullptr;
	int up_anim_id = 0;
	int up_right_anim_id = 0;
	int right_anim_id = 0;
	int down_right_anim_id = 0;
	int down_anim_id = 0;
	int down_left_anim_id = 0;
	int left_anim_id = 0;
	int up_left_anim_id = 0;
	int anim_frame_time = 100;
public:
	PlayerController(bool& is_game_running) : is_game_running(is_game_running), x(0), y(0) {}
	virtual ~PlayerController() = default;
	void Setup() override;
	void Update(float delta_time) override;
	void SetMoveSpeed(float move_speed) { this->move_speed = move_speed; }
};