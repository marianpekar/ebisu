#include "character_animator.h"

#include <iostream>

#include "animator.h"
#include "transform.h"
#include "../entity.h"

void CharacterAnimator::Setup()
{
    idle_anim = std::make_shared<CharacterAnimation>(idle_anim_start_frame, idle_anim_end_frame, idle_anim_frame_time, animator);
    move_anim = std::make_shared<CharacterAnimation>(move_anim_start_frame, move_anim_end_frame, move_anim_frame_time, animator);
    
    current_idle_animation_id = idle_anim->down_anim_id;
    animator->Play(current_idle_animation_id);
}

void CharacterAnimator::Update(float delta_time)
{
    static constexpr float dir_change_threshold = 0.001f;
    
    if (move_dir.Length() > dir_change_threshold)
    {
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
