#pragma once

#include "component.h"

#include <vector>

struct Animation 
{
	Animation(int row, int frames, float frame_time, bool is_loop) : row(row), frames(frames), frame_time(frame_time), is_loop(is_loop) {};
	int id = 0;
	int row;
	int frames;
	int frame_time;
	int current_frame = 0;
	bool is_loop = false;
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
	int AddAnimation(const int row, const int frames, const float speed, const bool is_loop);
	void Play(const int id);
	void Stop();
	void StopImmediately();
};