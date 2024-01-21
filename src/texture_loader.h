#pragma once

#include <string>
#include <unordered_map>

class TextureLoader 
{
private:
    static std::unordered_map<std::string, struct SDL_Texture*> texture_cache;
public:
    static SDL_Texture* LoadTexture(const std::string& filepath, struct SDL_Renderer* renderer, int& width, int& height);
    static void ClearTextureCache();
};