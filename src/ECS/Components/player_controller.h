#pragma once

#include "../../Math/vector2.h"
#include "component.h"
#include <queue>

class PlayerController : public Component
{
private:
	class Game* game;
	float move_speed = 4096.f;
	Vector2 move_dir;
	class Animator* animator = nullptr;
	class Rigidbody* rigidbody = nullptr;
	class BoxCollider* box_collider = nullptr;
	std::queue<BoxCollider*> other_colliders;

	size_t up_anim_id = 0;
	size_t up_right_anim_id = 0;
	size_t right_anim_id = 0;
	size_t down_right_anim_id = 0;
	size_t down_anim_id = 0;
	size_t down_left_anim_id = 0;
	size_t left_anim_id = 0;
	size_t up_left_anim_id = 0;
	int move_anim_frame_time = 300;

	size_t idle_up_anim_id = 0;
	size_t idle_up_right_anim_id = 0;
	size_t idle_right_anim_id = 0;
	size_t idle_down_right_anim_id = 0;
	size_t idle_down_anim_id = 0;
	size_t idle_down_left_anim_id = 0;
	size_t idle_left_anim_id = 0;
	size_t idle_up_left_anim_id = 0;
	int idle_anim_frame_time = 600;

	size_t current_idle_animation_id = 0;

public:
	PlayerController(Game* game) : game(game) {}
	~PlayerController() override = default;
	void Setup() override;
	void Update(float delta_time) override;
	void SetMoveSpeed(float player_move_speed);
};