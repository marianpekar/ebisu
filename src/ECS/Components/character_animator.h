#pragma once
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

    CharacterAnimation(const int start_frame, const int end_frame, const int frame_time, Animator* animator) :
        start_frame(start_frame), end_frame(end_frame), frame_time(frame_time)
    {
        up_anim_id = animator->AddAnimation(0, start_frame, end_frame, frame_time, true, false);
        up_right_anim_id = animator->AddAnimation(1, start_frame, end_frame, frame_time, true, false);
        right_anim_id = animator->AddAnimation(2, start_frame, end_frame, frame_time, true, false);
        down_right_anim_id = animator->AddAnimation(3, start_frame, end_frame, frame_time, true, false);
        down_anim_id = animator->AddAnimation(4, start_frame, end_frame, frame_time, true, false);
        down_left_anim_id = animator->AddAnimation(5, start_frame, end_frame, frame_time, true, false);
        left_anim_id = animator->AddAnimation(6, start_frame, end_frame, frame_time, true, false);
        up_left_anim_id = animator->AddAnimation(7, start_frame, end_frame, frame_time, true, false);
    }
};

class CharacterAnimator final : public Component
{
private:
    Animator* animator = nullptr;
    class Transform* transform = nullptr;

    CharacterAnimation* idle_anim;
    CharacterAnimation* move_anim;

    int move_anim_start_frame;
    int move_anim_end_frame;
    float move_anim_frame_time;
    int idle_anim_start_frame;
    int idle_anim_end_frame;
    float idle_anim_frame_time;

    size_t current_idle_animation_id = 0;
    float idle_move_anim_threshold = 0.0f;
    Vector2 last_position = Vector2(0.0f, 0.0f);

public:
    CharacterAnimator(Animator* animator, const float idle_to_move_anim_threshold,
                      const int move_anim_start_frame, const int move_anim_end_frame, const float move_anim_frame_time,
                      const int idle_anim_start_frame, const int idle_anim_end_frame, const float idle_anim_frame_time) :
    animator(animator), idle_anim(nullptr), move_anim(nullptr),
    move_anim_start_frame(move_anim_start_frame), move_anim_end_frame(move_anim_end_frame), move_anim_frame_time(move_anim_frame_time),
    idle_anim_start_frame(idle_anim_start_frame), idle_anim_end_frame(idle_anim_end_frame), idle_anim_frame_time(idle_anim_frame_time),
    idle_move_anim_threshold(idle_to_move_anim_threshold) {}

    ~CharacterAnimator() override
    {
        delete idle_anim;
        delete move_anim;
    }

    void Setup() override;
    void Update(float delta_time) override;
};
