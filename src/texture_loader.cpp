#include "texture_loader.h"
#include <SDL.h>
#include <SDL_image.h>

SDL_Texture* TextureLoader::LoadTexture(const char* filepath, SDL_Renderer* renderer, int& width, int& height)
{
	SDL_Surface* surface = IMG_Load(filepath);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	width = surface->w;
	height = surface->h;
	SDL_FreeSurface(surface);
	return texture;
}
