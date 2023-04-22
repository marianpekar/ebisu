#pragma once

#include "../../Math/vector2.h"
#include "component.h"

class PlayerController : public Component
{
private:
	bool& is_game_running;
	float move_speed = 300.0f;
	Vector2 move_dir;
	class Animator* animator = nullptr;
	class Transform* transform = nullptr;
	class MapCollider* map_collider = nullptr;
	class BoxCollider* box_collider = nullptr;

	int up_anim_id = 0;
	int up_right_anim_id = 0;
	int right_anim_id = 0;
	int down_right_anim_id = 0;
	int down_anim_id = 0;
	int down_left_anim_id = 0;
	int left_anim_id = 0;
	int up_left_anim_id = 0;
	int move_anim_frame_time = 300;

	int idle_up_anim_id = 0;
	int idle_up_right_anim_id = 0;
	int idle_right_anim_id = 0;
	int idle_down_right_anim_id = 0;
	int idle_down_anim_id = 0;
	int idle_down_left_anim_id = 0;
	int idle_left_anim_id = 0;
	int idle_up_left_anim_id = 0;
	int idle_anim_frame_time = 600;

	int current_idle_animation_id = 0;
public:
	PlayerController(bool& is_game_running) : is_game_running(is_game_running), move_dir() {}
	virtual ~PlayerController() = default;
	void Setup() override;
	void Update(float delta_time) override;
	void SetMoveSpeed(float move_speed) { this->move_speed = move_speed; }
};