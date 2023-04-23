#include <SDL.h>
#include <algorithm>
#include "../entity.h"
#include "player_controller.h"
#include "transform.h"
#include "animator.h"
#include "map_collider.h"
#include "box_collider.h"
#include "rigidbody.h"

void PlayerController::Setup()
{
	rigidbody = owner->GetComponent<Rigidbody>();
	animator = owner->GetComponent<Animator>();
	box_collider = owner->GetComponent<BoxCollider>();

	box_collider->on_collision = [this](BoxCollider* other) {
		other_colliders.push(other);
	};

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

	Vector2 move_force = move_dir * move_speed;
	rigidbody->AddForce(move_force);
	
	while(other_colliders.size() > 0)
	{
		// on collision with each box collider

		other_colliders.pop();
	}
}
