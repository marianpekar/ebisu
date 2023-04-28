#pragma once

#include <unordered_map>

class TextureLoader 
{
private:
	static std::unordered_map<const char*, struct SDL_Texture*> texture_cache;
public:
	static SDL_Texture* LoadTexture(const char* filepath, struct SDL_Renderer* renderer, int& width, int& height);
	static SDL_Texture* LoadTexture(const char* filepath, SDL_Renderer* renderer);
};