#include "../entity.h"
#include "animator.h"
#include "sprite_sheet.h"
#include <SDL.h>

void Animator::Setup()
{
    sprite_sheet = owner->GetComponent<SpriteSheet>();

    if (active_anim_id != no_id)
    {
        sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->start_frame);
        animations[active_anim_id]->is_running = true;
    }
}

void Animator::Update(float delta_time)
{
    if (active_anim_id == no_id)
        return;

    const Uint32 current_time = SDL_GetTicks();

    if (const Uint32 elapsed_time = current_time - last_time; elapsed_time >= static_cast<Uint32>(animations[active_anim_id]->frame_time))
    {
        last_time = current_time;

        ++animations[active_anim_id]->current_frame;

        const bool is_over_last_frame = animations[active_anim_id]->current_frame > animations[active_anim_id]->end_frame;
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

size_t Animator::AddAnimation(const int row, const int start_frame, const int end_frame, const int frame_time, const bool is_loop, const bool play_on_setup)
{
    auto animation = new Animation(row, start_frame, end_frame, frame_time, is_loop);
    animation->id = animations.size();
    animations.emplace_back(animation);

    if (play_on_setup)
    {
        active_anim_id = animation->id;
    }

    return animation->id;
}

void Animator::Play(const size_t id)
{
    if (active_anim_id == id)
        return;

    if (active_anim_id != no_id)
    {
        animations[active_anim_id]->is_running = false;
        animations[active_anim_id]->current_frame = animations[active_anim_id]->start_frame;
    }

    active_anim_id = id;
    sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->start_frame);
    animations[active_anim_id]->is_running = true;
}

void Animator::Stop() const
{
    if (active_anim_id == no_id)
        return;

    animations[active_anim_id]->is_running = false;
}

void Animator::StopImmediately()
{
    if (active_anim_id == no_id)
        return;

    animations[active_anim_id]->is_running = false;
    animations[active_anim_id]->current_frame = animations[active_anim_id]->start_frame;
    sprite_sheet->SelectSprite(animations[active_anim_id]->row, animations[active_anim_id]->start_frame);
    active_anim_id = -1;
}