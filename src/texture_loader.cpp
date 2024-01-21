#include "texture_loader.h"
#include <SDL.h>
#include <SDL_image.h>

std::unordered_map<std::string, SDL_Texture*> TextureLoader::texture_cache;

SDL_Texture* TextureLoader::LoadTexture(const std::string& filepath, SDL_Renderer* renderer, int& width, int& height)
{
    if (texture_cache.contains(filepath))
        return texture_cache[filepath];

    SDL_Surface* surface = IMG_Load(filepath.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    width = surface->w;
    height = surface->h;
    SDL_FreeSurface(surface);

    texture_cache.emplace(filepath, texture);

    return texture;
}

void TextureLoader::ClearTextureCache()
{
    texture_cache.clear();
}

