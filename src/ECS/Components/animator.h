#pragma once

#include "component.h"

#include <vector>

struct Animation 
{
	Animation(int row, int start_frame, int end_frame, float frame_time, bool is_loop) : 
		row(row), frame_time(frame_time), is_loop(is_loop), start_frame(start_frame),
		frames(end_frame - start_frame), current_frame(start_frame), 
		end_frame(start_frame + (end_frame - start_frame)) {};
	int id = 0;
	int row;
	int frames;
	int frame_time;
	int start_frame;
	int current_frame;
	int end_frame;
	bool is_loop;
	bool is_running = false;
};

class Animator : public Component
{
private:
	class SpriteSheet* sprite_sheet = nullptr;
	std::vector<Animation*> animations;
	int active_anim_id = -1;
	int last_time = 0;
public:
	Animator() = default;
	virtual ~Animator() = default;
	void Setup() override;
	void Update(float deltaTime) override;
	int AddAnimation(const int row, const int start_frame, const int end_frame, const float frame_time, const bool is_loop);
	void Play(const int id);
	void Stop();
	void StopImmediately();
};