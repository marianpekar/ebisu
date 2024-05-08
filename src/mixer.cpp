#include "mixer.h"
#include <iostream>

std::unordered_map<std::string, Mix_Chunk*> Mixer::sound_cache;

bool Mixer::Initialize()
{
    if (Mix_Init(0) != 0)
    {
        std::cout << "[Mixer] Failed to initialize SDL Mixer" << '\n';
        return false;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
    {
        std::cout << "[Mixer] Failed to open audio device" << '\n';
        return false;
    }
    
    return true;
}

void Mixer::PlayAudio(const std::string& filepath, const int loops)
{
    Mix_PlayChannel(-1, sound_cache.at(filepath), loops);
}

void Mixer::StopAllAudio()
{
    Mix_HaltChannel(-1);
}

void Mixer::LoadAudio(const std::string& filepath)
{
    if (sound_cache.contains(filepath))
        return;
    
    Mix_Chunk* sound = Mix_LoadWAV(filepath.c_str());

    if (!sound)
    {
        std::cout << "[Mixer] Failed to load sound from '" << filepath << "'\n" << Mix_GetError() << '\n';
        throw;
    }
    
    sound_cache.try_emplace(filepath, sound);
}

void Mixer::Destroy()
{
    Mix_CloseAudio();

    for (auto& [_, chunk] : sound_cache)
    {
        Mix_FreeChunk(chunk);
    }
    
    sound_cache.clear();
}
