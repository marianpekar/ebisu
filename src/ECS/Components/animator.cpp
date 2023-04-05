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
			
		bool is_over_last_frame = animations[active_anim_id]->current_frame > animations[active_anim_id]->end_frame;
		if (is_over_last_frame && animations[active_anim_id]->is_loop)
		{
			animations[active_anim_id]->current_frame = animations[active_anim_id]->start_frame;
		}

		if (is_over_last_frame && (!animations[active_anim_id]->is_loop || !animations[active_anim_id]->is_running))
		{
			animations[active_anim_id]->is_running = false;
			active_anim_id = -1;
			return;
		}

		sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->current_frame);
	}
}

int Animator::AddAnimation(int row, int start_frame, int end_frame, float frame_time, bool is_loop)
{
	Animation* animation = new Animation(row, start_frame, end_frame, frame_time, is_loop);
	animation->id = animations.size();
	animations.emplace_back(animation);
	return animation->id;
}

void Animator::Play(int id)
{
	if (active_anim_id == id)
		return;

	if (active_anim_id != -1)
	{
		animations[active_anim_id]->is_running = false;
		animations[active_anim_id]->current_frame = animations[active_anim_id]->start_frame;
	}

	active_anim_id = id;
	sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->start_frame);
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
	animations[active_anim_id]->current_frame = animations[active_anim_id]->start_frame;
	sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->start_frame);
	active_anim_id = -1;
}