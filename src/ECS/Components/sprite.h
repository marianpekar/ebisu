#pragma once

#include "component.h"

class Sprite : public Component
{
private:
	int width, height;
	struct SDL_Texture* sprite;
	struct SDL_Renderer* renderer;
	struct SDL_Rect* dst_rect;
	class Transform* transform;
public:
	Sprite() = default;
	virtual ~Sprite() = default;
	void Setup(const char* filepath, SDL_Renderer* renderer);
	void Update(float delta_time) override;
	void Render() override;
};