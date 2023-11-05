#pragma once
#include "component.h"
#include "../../Math/vector2.h"

class CharacterAnimator final : public Component
{
private:
    class Animator* animator = nullptr;
    class Transform* transform = nullptr;
    
    size_t up_anim_id = 0;
    size_t up_right_anim_id = 0;
    size_t right_anim_id = 0;
    size_t down_right_anim_id = 0;
    size_t down_anim_id = 0;
    size_t down_left_anim_id = 0;
    size_t left_anim_id = 0;
    size_t up_left_anim_id = 0;
    int move_anim_frame_time = 300;

    size_t idle_up_anim_id = 0;
    size_t idle_up_right_anim_id = 0;
    size_t idle_right_anim_id = 0;
    size_t idle_down_right_anim_id = 0;
    size_t idle_down_anim_id = 0;
    size_t idle_down_left_anim_id = 0;
    size_t idle_left_anim_id = 0;
    size_t idle_up_left_anim_id = 0;
    int idle_anim_frame_time = 600;

    size_t current_idle_animation_id = 0;

    Vector2 last_position;
public:
    CharacterAnimator(Animator* animator) : animator(animator) {}
    ~CharacterAnimator() override = default;
    void Setup() override;
    void Update(float delta_time) override;
};
