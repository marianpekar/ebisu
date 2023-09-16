#pragma once

#include "component.h"

class Sprite : public Component
{
private:
	int width, height;
	struct SDL_Renderer* renderer;
	const char* filepath;
	struct SDL_Texture* sprite = nullptr;
	struct SDL_Rect* dst_rect = nullptr;
	class Transform* transform = nullptr;
	class Camera* camera = nullptr;
public:
	Sprite(const char* filepath, SDL_Renderer* renderer, Camera* camera) :
		width(0), height(0), renderer(renderer), filepath(filepath), camera(camera) {};
	~Sprite() override = default;
	void Setup() override;
	void Render() override;
};