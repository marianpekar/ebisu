#pragma once

#include "component.h"
#include <string>

class AudioSource : public Component
{
private:
    std::string filepath;
    int loops;
    bool play_on_start;
public:
    AudioSource(std::string filepath, const int loops, const bool play_on_start) : filepath(std::move(filepath)), loops(loops), play_on_start(play_on_start) {}
    void Setup() override;
    void Play() const;
};
