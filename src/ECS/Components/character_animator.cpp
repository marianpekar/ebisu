﻿#include "character_animator.h"

#include <iostream>

#include "animator.h"
#include "transform.h"
#include "../entity.h"

void CharacterAnimator::Setup()
{
    transform = owner->GetComponent<Transform>();
    last_position = transform->GetPosition();

    idle_anim = new CharacterAnimation(idle_anim_start_frame, idle_anim_end_frame, idle_anim_frame_time, animator);
    move_anim = new CharacterAnimation(move_anim_start_frame, move_anim_end_frame, move_anim_frame_time, animator);
    
    current_idle_animation_id = idle_anim->down_anim_id;
    animator->Play(current_idle_animation_id);
}

void CharacterAnimator::Update(float delta_time)
{
    const Vector2 current_position = transform->GetPosition();
    const Vector2 move_dir = current_position - last_position;
    const float speed = move_dir.Length() / delta_time;
    last_position = current_position;
    
    if (speed > idle_move_anim_threshold)
    {
        constexpr float dir_change_threshold = 0.1f;
        
        if (move_dir.y < -dir_change_threshold)
        {
            if (move_dir.x < -dir_change_threshold)
            {
                animator->Play(move_anim->up_left_anim_id);
                current_idle_animation_id = idle_anim->up_left_anim_id;
            }
            else if (move_dir.x > dir_change_threshold)
            {
                animator->Play(move_anim->up_right_anim_id);
                current_idle_animation_id = idle_anim->up_right_anim_id;
            }
            else
            {
                animator->Play(move_anim->up_anim_id);
                current_idle_animation_id = idle_anim->up_anim_id;
            }
        }
        else if (move_dir.y > dir_change_threshold)
        {
            if (move_dir.x < -dir_change_threshold)
            {
                animator->Play(move_anim->down_left_anim_id);
                current_idle_animation_id = idle_anim->down_left_anim_id;
            }
            else if (move_dir.x > dir_change_threshold)
            {
                animator->Play(move_anim->down_right_anim_id);
                current_idle_animation_id = idle_anim->down_right_anim_id;
            }
            else
            {
                animator->Play(move_anim->down_anim_id);
                current_idle_animation_id = idle_anim->down_anim_id;
            }
        }
        else if (move_dir.x < -dir_change_threshold)
        {
            animator->Play(move_anim->left_anim_id);
            current_idle_animation_id = idle_anim->left_anim_id;
        }
        else if (move_dir.x > dir_change_threshold)
        {
            animator->Play(move_anim->right_anim_id);
            current_idle_animation_id = idle_anim->right_anim_id;
        }
    }
    else
    {
        animator->Play(current_idle_animation_id);
    }
}