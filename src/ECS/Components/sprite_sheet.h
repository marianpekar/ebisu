#pragma once

#include "component.h"

class SpriteSheet : public Component
{
private:
	int sheet_width, sheet_height, rect_width, rect_height;
	struct SDL_Texture* sprite = nullptr;
	struct SDL_Renderer* renderer;
	struct SDL_Rect* src_rect = nullptr;
	SDL_Rect* dst_rect = nullptr;
	const char* filepath;
	class Transform* transform = nullptr;
public:
	SpriteSheet(const char* filepath, SDL_Renderer* renderer, int rect_width, int rect_height) : 
		renderer(renderer), rect_width(rect_width), rect_height(rect_height),
		filepath(filepath), sheet_width(0), sheet_height(0) {};
	virtual ~SpriteSheet() = default;
	void Setup() override;
	void Render() override;
	void SelectSprite(const int& row, const int& col);
};