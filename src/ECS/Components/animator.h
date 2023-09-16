#pragma once

#include <SDL.h>
#include "component.h"
#include <vector>

struct Animation 
{
    Animation(const int row, const int start_frame, const int end_frame, const int frame_time, const bool is_loop) : 
        row(row), frames(end_frame - start_frame), frame_time(frame_time), start_frame(start_frame),
        current_frame(start_frame), end_frame(start_frame + (end_frame - start_frame)),
        is_loop(is_loop) {}
    size_t id = 0;
    int row;
    int frames;
    int frame_time;
    int start_frame;
    int current_frame;
    int end_frame;
    bool is_loop;
    bool is_running = false;
};

class Animator final : public Component
{
private:
    class SpriteSheet* sprite_sheet = nullptr;
    std::vector<Animation*> animations;
    size_t active_anim_id = -1;
    size_t no_id = -1;
    Uint32 last_time = 0;
public:
    void Setup() override;
    void Update(float delta_time) override;
    size_t AddAnimation(int row, int start_frame, int end_frame, int frame_time, bool is_loop, bool play_on_setup);
    void Play(size_t id);
    void Stop() const;
    void StopImmediately();
};