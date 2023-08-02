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
	std::string filepath;
	class Transform* transform = nullptr;
	class Camera* camera;
public:
	SpriteSheet(std::string filepath, SDL_Renderer* renderer, int rect_width, int rect_height, Camera* camera);
	~SpriteSheet();
	void Setup() override;
	void Render() override;
	void SelectSprite(const int& row, const int& col);
};