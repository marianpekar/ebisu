#include <SDL.h>
#include "../entity.h"
#include "player_controller.h"
#include "transform.h"
#include "animator.h"

void PlayerController::Setup()
{
	transform = owner->GetComponent<Transform>();
	animator = owner->GetComponent<Animator>();

	up_anim_id = animator->AddAnimation(0, 5, anim_frame_time, true);
	up_right_anim_id = animator->AddAnimation(1, 5, anim_frame_time, true);
	right_anim_id = animator->AddAnimation(2, 5, anim_frame_time, true);
	down_right_anim_id = animator->AddAnimation(3, 5, anim_frame_time, true);
	down_anim_id = animator->AddAnimation(4, 5, anim_frame_time, true);
	down_left_anim_id = animator->AddAnimation(5, 5, anim_frame_time, true);
	left_anim_id = animator->AddAnimation(6, 5, anim_frame_time, true);
	up_left_anim_id = animator->AddAnimation(7, 5, anim_frame_time, true);
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
			}
			// down
			else if (x == 0 && y > 0)
			{
				animator->Play(down_anim_id);
			}
			// left
			else if (x < 0 && y == 0)
			{
				animator->Play(left_anim_id);
			}
			// right
			else if (x > 0 && y == 0)
			{
				animator->Play(right_anim_id);
			}
			// up left
			else if (x < 0 && y < 0)
			{
				x = -0.707f;
				y = -0.707f;
				animator->Play(up_left_anim_id);
			}
			// up right
			else if (x > 0 && y < 0)
			{
				x = 0.707f;
				y = -0.707f;
				animator->Play(up_right_anim_id);
			}
			// down left
			else if (x < 0 && y > 0)
			{
				x = -0.707f;
				y = 0.707f;
				animator->Play(down_left_anim_id);
			}
			// down right
			else if (x > 0 && y > 0)
			{
				x = 0.707f;
				y = 0.707f;
				animator->Play(down_right_anim_id);
			}
			else if (x == 0 && y == 0)
			{
				animator->StopImmediately();
			}	
			
			break;
		default:
			break;
	}

	transform->Move(x * move_speed * delta_time, y * move_speed * delta_time);
}
