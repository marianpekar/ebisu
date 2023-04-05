#pragma once

#include "component.h"

class Sprite : public Component
{
private:
	int width, height;
	struct SDL_Texture* sprite;
	struct SDL_Renderer* renderer;
	struct SDL_Rect* dst_rect;
public:
	Sprite() = default;
	virtual ~Sprite() = default;
	void Setup(const char* filepath, SDL_Renderer* renderer);
	void Render() override;
};