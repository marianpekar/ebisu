#include "audio_source.h"
#include "../../mixer.h"

void AudioSource::Setup()
{
    Mixer::LoadAudio(filepath);

    if (play_on_start)
        Play();
}

void AudioSource::Play() const
{
    Mixer::PlayAudio(filepath, loops);
}
