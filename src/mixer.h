#pragma once
#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class Mixer
{
private:
    static std::unordered_map<std::string, Mix_Chunk*> sound_cache;
public:
    static bool Initialize();
    static void LoadAudio(const std::string& filepath);
    static void PlayAudio(const std::string& filepath, int loops = 0);
    static void Destroy();
};
