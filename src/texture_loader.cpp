#include "texture_loader.h"
#include <SDL.h>
#include <SDL_image.h>

std::unordered_map<const char*, SDL_Texture*> TextureLoader::texture_cache;

SDL_Texture* TextureLoader::LoadTexture(const char* filepath, SDL_Renderer* renderer, int& width, int& height)
{
	if (texture_cache.contains(filepath))
		return texture_cache[filepath];

	SDL_Surface* surface = IMG_Load(filepath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	width = surface->w;
	height = surface->h;
	SDL_FreeSurface(surface);

	texture_cache[filepath] = texture;

	return texture;
}

SDL_Texture* TextureLoader::LoadTexture(const char* filepath, SDL_Renderer* renderer)
{
	SDL_Surface* surface = IMG_Load(filepath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}
