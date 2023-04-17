#include <SDL.h>
#include "../entity.h"
#include "player_controller.h"
#include "transform.h"
#include "animator.h"
#include "map_collider.h"
#include "box_collider.h"

void PlayerController::Setup()
{
	transform = owner->GetComponent<Transform>();
	animator = owner->GetComponent<Animator>();
	map_collider = owner->GetComponent<MapCollider>();
	box_collider = owner->GetComponent<BoxCollider>();

	const int move_start_anim_frame = 2;
	const int move_end_anim_frame = 5;

	up_anim_id = animator->AddAnimation(0, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	up_right_anim_id = animator->AddAnimation(1, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	right_anim_id = animator->AddAnimation(2, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	down_right_anim_id = animator->AddAnimation(3, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	down_anim_id = animator->AddAnimation(4, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	down_left_anim_id = animator->AddAnimation(5, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	left_anim_id = animator->AddAnimation(6, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
	up_left_anim_id = animator->AddAnimation(7, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);

	const int idle_start_anim_frame = 0;
	const int idle_end_anim_frame = 1;

	idle_up_anim_id = animator->AddAnimation(0, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_up_right_anim_id = animator->AddAnimation(1, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_right_anim_id = animator->AddAnimation(2, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_down_right_anim_id = animator->AddAnimation(3, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_down_anim_id = animator->AddAnimation(4, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_down_left_anim_id = animator->AddAnimation(5, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_left_anim_id = animator->AddAnimation(6, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);
	idle_up_left_anim_id = animator->AddAnimation(7, idle_start_anim_frame, idle_end_anim_frame, idle_anim_frame_time, true, false);

	current_idle_animation_id = idle_up_anim_id;
	animator->Play(current_idle_animation_id);
}

void PlayerController::Update(float delta_time)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	switch (event.type)
	{
		case SDL_QUIT:
			is_game_running = false;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if (keys[SDL_SCANCODE_W])
			{
				y = -1;
			}
			else if (keys[SDL_SCANCODE_S])
			{
				y = 1;
			}
			else
			{
				y = 0;
			}

			if (keys[SDL_SCANCODE_A])
			{
				x = -1;
			}
			else if (keys[SDL_SCANCODE_D])
			{
				x = 1;
			}
			else
			{
				x = 0;
			}

			// up
			if (x == 0 && y < 0)
			{
				animator->Play(up_anim_id);
				current_idle_animation_id = idle_up_anim_id;
			}
			// down
			else if (x == 0 && y > 0)
			{
				animator->Play(down_anim_id);
				current_idle_animation_id = idle_down_anim_id;
			}
			// left
			else if (x < 0 && y == 0)
			{
				animator->Play(left_anim_id);
				current_idle_animation_id = idle_left_anim_id;
			}
			// right
			else if (x > 0 && y == 0)
			{
				animator->Play(right_anim_id);
				current_idle_animation_id = idle_right_anim_id;
			}
			// up left
			else if (x < 0 && y < 0)
			{
				x = -0.707f;
				y = -0.707f;
				animator->Play(up_left_anim_id);
				current_idle_animation_id = idle_up_left_anim_id;
			}
			// up right
			else if (x > 0 && y < 0)
			{
				x = 0.707f;
				y = -0.707f;
				animator->Play(up_right_anim_id);
				current_idle_animation_id = idle_up_right_anim_id;
			}
			// down left
			else if (x < 0 && y > 0)
			{
				x = -0.707f;
				y = 0.707f;
				animator->Play(down_left_anim_id);
				current_idle_animation_id = idle_down_left_anim_id;
			}
			// down right
			else if (x > 0 && y > 0)
			{
				x = 0.707f;
				y = 0.707f;
				animator->Play(down_right_anim_id);
				current_idle_animation_id = idle_down_right_anim_id;
			}
			else if (x == 0 && y == 0)
			{
				animator->Play(current_idle_animation_id);
			}	
			
			break;
		default:
			break;
	}

	float current_x = transform->GetX();
	float current_y = transform->GetY();

	float target_x = current_x + x * move_speed * delta_time;
	float target_y = current_y + y * move_speed * delta_time;

	for (const auto& other : box_collider->GetOthers())
	{		
		Transform* other_transform = other.first->owner->GetComponent<Transform>();

		float dir_x = other_transform->GetX() - current_x;
		float dir_y = other_transform->GetY() - current_y;

		float mag = std::sqrt(dir_x * dir_x + dir_y * dir_y);
		if (mag > 0) {
			float inv_mag = 1.0f / mag;
			dir_x *= inv_mag;
			dir_y *= inv_mag;
		}

		float push_back_dist = 7.f;
		float push_x = dir_x * push_back_dist;
		float push_y = dir_y * push_back_dist;

		target_x = target_x - push_x;
		target_y = target_y - push_y;
	}

	if (map_collider->HasCollisionAt(target_x, current_y))
	{
		target_x = current_x;
	}

	if (map_collider->HasCollisionAt(current_x, target_y))
	{
		target_y = current_y;
	}

	transform->SetPosition(target_x, target_y);
}
