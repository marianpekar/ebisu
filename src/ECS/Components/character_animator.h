#pragma once

#include <memory>
#include "animator.h"
#include "component.h"
#include "../../Math/vector2.h"

struct CharacterAnimation
{
    size_t up_anim_id = 0;
    size_t up_right_anim_id = 0;
    size_t right_anim_id = 0;
    size_t down_right_anim_id = 0;
    size_t down_anim_id = 0;
    size_t down_left_anim_id = 0;
    size_t left_anim_id = 0;
    size_t up_left_anim_id = 0;
    int start_frame;
    int end_frame;
    int frame_time;

    CharacterAnimation(const int start_frame, const int end_frame, const int frame_time, const bool is_loop, const std::shared_ptr<Animator>& animator) :
        start_frame(start_frame), end_frame(end_frame), frame_time(frame_time)
    {
        up_anim_id = animator->AddAnimation(0, start_frame, end_frame, frame_time, is_loop, false);
        up_right_anim_id = animator->AddAnimation(1, start_frame, end_frame, frame_time, is_loop, false);
        right_anim_id = animator->AddAnimation(2, start_frame, end_frame, frame_time, is_loop, false);
        down_right_anim_id = animator->AddAnimation(3, start_frame, end_frame, frame_time, is_loop, false);
        down_anim_id = animator->AddAnimation(4, start_frame, end_frame, frame_time, is_loop, false);
        down_left_anim_id = animator->AddAnimation(5, start_frame, end_frame, frame_time, is_loop, false);
        left_anim_id = animator->AddAnimation(6, start_frame, end_frame, frame_time, is_loop, false);
        up_left_anim_id = animator->AddAnimation(7, start_frame, end_frame, frame_time, is_loop, false);
    }
};

class CharacterAnimator final : public Component
{
private:
    std::shared_ptr<Animator> animator;
    
    std::shared_ptr<CharacterAnimation> idle_anim;
    std::shared_ptr<CharacterAnimation> move_anim;
    std::shared_ptr<CharacterAnimation> death_anim;

    int move_anim_start_frame;
    int move_anim_end_frame;
    float move_anim_frame_time;
    
    int idle_anim_start_frame;
    int idle_anim_end_frame;
    float idle_anim_frame_time;

    int death_anim_start_frame;
    int death_anim_end_frame;
    float death_anim_frame_time;

    size_t current_idle_animation_id = 0;
    size_t current_death_animation_id = 0;

    Vector2 move_dir = Vector2(0.0f, 0.0f);

    bool is_dead = false;
    
public:
    CharacterAnimator(const std::shared_ptr<Animator>& animator,
                      const int move_anim_start_frame, const int move_anim_end_frame, const float move_anim_frame_time,
                      const int idle_anim_start_frame, const int idle_anim_end_frame, const float idle_anim_frame_time,
                      const int death_anim_start_frame, const int death_anim_end_frame, const float death_anim_frame_time) :
    animator(animator),
    idle_anim(nullptr), move_anim(nullptr),
    move_anim_start_frame(move_anim_start_frame), move_anim_end_frame(move_anim_end_frame), move_anim_frame_time(move_anim_frame_time),
    idle_anim_start_frame(idle_anim_start_frame), idle_anim_end_frame(idle_anim_end_frame), idle_anim_frame_time(idle_anim_frame_time),
    death_anim_start_frame(death_anim_start_frame), death_anim_end_frame(death_anim_end_frame), death_anim_frame_time(idle_anim_frame_time) {}

    ~CharacterAnimator() override = default;

    void SetMoveDirection(const Vector2& direction) { move_dir = direction; }

    void Setup() override;
    void Update(float delta_time) override;

    void PlayDeadAnimation();
    void PlayDeadAnimationLastFrame();
    const size_t& GetCurrentDeathAnimId() const { return current_death_animation_id; }
    void SetCurrentDeathAnimId(const size_t& id) { current_death_animation_id = id; }
};
