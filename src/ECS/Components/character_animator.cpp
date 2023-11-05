#include "character_animator.h"
#include "animator.h"
#include "transform.h"
#include "../entity.h"

void CharacterAnimator::Setup()
{
    transform = owner->GetComponent<Transform>();

    // TODO: expose to level data and pass as parameters
    constexpr int move_start_anim_frame = 2;
    constexpr int move_end_anim_frame = 5;

    up_anim_id = animator->AddAnimation(0, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    up_right_anim_id = animator->AddAnimation(1, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    right_anim_id = animator->AddAnimation(2, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    down_right_anim_id = animator->AddAnimation(3, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    down_anim_id = animator->AddAnimation(4, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    down_left_anim_id = animator->AddAnimation(5, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    left_anim_id = animator->AddAnimation(6, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);
    up_left_anim_id = animator->AddAnimation(7, move_start_anim_frame, move_end_anim_frame, move_anim_frame_time, true, false);

    // TODO: expose to level data and pass as parameters
    constexpr int idle_start_anim_frame = 0;
    constexpr int idle_end_anim_frame = 1;

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

    last_position = transform->GetPosition();
}

void CharacterAnimator::Update(float delta_time)
{
    //TODO: animation switching based on delta position needs some work to look better
    
    const Vector2 current_position = transform->GetPosition();
    const Vector2 move_dir = current_position - last_position;
    last_position = current_position;
    
    constexpr float threshold = 0.1f;

    // up
    if (move_dir.y < -threshold)
    {
        if (move_dir.x < -threshold)
        {
            animator->Play(up_left_anim_id);
            current_idle_animation_id = idle_up_left_anim_id;
        }
        else if (move_dir.x > threshold)
        {
            animator->Play(up_right_anim_id);
            current_idle_animation_id = idle_up_right_anim_id;
        }
        else
        {
            animator->Play(up_anim_id);
            current_idle_animation_id = idle_up_anim_id;
        }
    }
    // down
    else if (move_dir.y > threshold)
    {
        if (move_dir.x < -threshold)
        {
            animator->Play(down_left_anim_id);
            current_idle_animation_id = idle_down_left_anim_id;
        }
        else if (move_dir.x > threshold)
        {
            animator->Play(down_right_anim_id);
            current_idle_animation_id = idle_down_right_anim_id;
        }
        else
        {
            animator->Play(down_anim_id);
            current_idle_animation_id = idle_down_anim_id;
        }
    }
    // left
    else if (move_dir.x < -threshold)
    {
        animator->Play(left_anim_id);
        current_idle_animation_id = idle_left_anim_id;
    }
    // right
    else if (move_dir.x > threshold)
    {
        animator->Play(right_anim_id);
        current_idle_animation_id = idle_right_anim_id;
    }
    else
    {
        animator->Play(current_idle_animation_id);
    }
}
