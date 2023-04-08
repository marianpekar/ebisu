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
		filepath(filepath), renderer(renderer), width(0), height(0), camera(camera) {};
	virtual ~Sprite() = default;
	void Setup() override;
	void Render() override;
};