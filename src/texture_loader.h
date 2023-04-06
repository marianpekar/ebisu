#pragma once

class TextureLoader 
{
public:
	static struct SDL_Texture* LoadTexture(const char* filepath, struct SDL_Renderer* renderer, int& width, int& height);
	static struct SDL_Texture* LoadTexture(const char* filepath, struct SDL_Renderer* renderer);
};