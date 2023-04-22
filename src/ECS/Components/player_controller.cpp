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
				move_dir.y = -1;
			}
			else if (keys[SDL_SCANCODE_S])
			{
				move_dir.y = 1;
			}
			else
			{
				move_dir.y = 0;
			}

			if (keys[SDL_SCANCODE_A])
			{
				move_dir.x = -1;
			}
			else if (keys[SDL_SCANCODE_D])
			{
				move_dir.x = 1;
			}
			else
			{
				move_dir.x = 0;
			}

			// up
			if (move_dir.x == 0 && move_dir.y < 0)
			{
				animator->Play(up_anim_id);
				current_idle_animation_id = idle_up_anim_id;
			}
			// down
			else if (move_dir.x == 0 && move_dir.y > 0)
			{
				animator->Play(down_anim_id);
				current_idle_animation_id = idle_down_anim_id;
			}
			// left
			else if (move_dir.x < 0 && move_dir.y == 0)
			{
				animator->Play(left_anim_id);
				current_idle_animation_id = idle_left_anim_id;
			}
			// right
			else if (move_dir.x > 0 && move_dir.y == 0)
			{
				animator->Play(right_anim_id);
				current_idle_animation_id = idle_right_anim_id;
			}
			// up left
			else if (move_dir.x < 0 && move_dir.y < 0)
			{
				move_dir.x = -0.707f;
				move_dir.y = -0.707f;
				animator->Play(up_left_anim_id);
				current_idle_animation_id = idle_up_left_anim_id;
			}
			// up right
			else if (move_dir.x > 0 && move_dir.y < 0)
			{
				move_dir.x = 0.707f;
				move_dir.y = -0.707f;
				animator->Play(up_right_anim_id);
				current_idle_animation_id = idle_up_right_anim_id;
			}
			// down left
			else if (move_dir.x < 0 && move_dir.y > 0)
			{
				move_dir.x = -0.707f;
				move_dir.y = 0.707f;
				animator->Play(down_left_anim_id);
				current_idle_animation_id = idle_down_left_anim_id;
			}
			// down right
			else if (move_dir.x > 0 && move_dir.y > 0)
			{
				move_dir.x = 0.707f;
				move_dir.y = 0.707f;
				animator->Play(down_right_anim_id);
				current_idle_animation_id = idle_down_right_anim_id;
			}
			else if (move_dir.x == 0 && move_dir.y == 0)
			{
				animator->Play(current_idle_animation_id);
			}	
			
			break;
		default:
			break;
	}

	Vector2 current_pos = transform->GetPosition();
	Vector2 target_pos = current_pos + move_dir * move_speed * delta_time;

	for (const auto& other : box_collider->GetOthers())
	{		
		Transform* other_transform = other.first->owner->GetComponent<Transform>();

		Vector2 dir = other_transform->GetPosition() - current_pos;

		float len = dir.Length();
		if (len > 0) {
			float inv_len = 1.0f / len;
			dir *= inv_len;
		}

		float push_back_dist = 7.f;
		Vector2 push = dir * push_back_dist;
		target_pos -= push;
	}

	if (map_collider->HasCollisionAt(Vector2(target_pos.x, current_pos.y)))
	{
		target_pos.x = current_pos.x;
	}

	if (map_collider->HasCollisionAt(Vector2(current_pos.x, target_pos.y)))
	{
		target_pos.y = current_pos.y;
	}

	transform->SetPosition(target_pos);
}
