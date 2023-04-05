#include "../entity.h"
#include "animator.h"
#include "sprite_sheet.h"
#include <SDL.h>

void Animator::Setup()
{
	sprite_sheet = owner->GetComponent<SpriteSheet>();
}

void Animator::Update(float deltaTime)
{
	if (active_anim_id == -1)
		return;

	Uint32 current_time = SDL_GetTicks();
	Uint32 elapsed_time = current_time - last_time;

	if (elapsed_time >= animations[active_anim_id]->frame_time)
	{
		last_time = current_time;

		++animations[active_anim_id]->current_frame;
			
		bool is_current_out_of_bounds = animations[active_anim_id]->current_frame == animations[active_anim_id]->frames;
		if (is_current_out_of_bounds)
		{
			animations[active_anim_id]->current_frame = 0;
		}

		sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->current_frame);

		if (is_current_out_of_bounds && (!animations[active_anim_id]->is_loop || !animations[active_anim_id]->is_running))
		{
			animations[active_anim_id]->is_running = false;
			active_anim_id = -1;
		}
	}
	
}

int Animator::AddAnimation(int row, int frames, float speed, bool is_loop)
{
	Animation* animation = new Animation(row, frames, speed, is_loop);
	animations.emplace_back(animation);
	animation->id = animations.size() - 1;
	return animation->id;
}

void Animator::Play(int id)
{
	if (active_anim_id == id)
		return;

	if (active_anim_id != -1)
	{
		animations[active_anim_id]->is_running = false;
		animations[active_anim_id]->current_frame = 0;
	}

	active_anim_id = id;
	animations[active_anim_id]->is_running = true;
}

void Animator::Stop()
{
	if (active_anim_id == -1)
		return;

	animations[active_anim_id]->is_running = false;
}

void Animator::StopImmediately()
{
	if (active_anim_id == -1)
		return;

	animations[active_anim_id]->is_running = false;
	animations[active_anim_id]->current_frame = 0;
	sprite_sheet->SelectSprite(animations[active_anim_id]->row, 0);
	active_anim_id = -1;
}
