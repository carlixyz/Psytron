#include "Audio.h"


bool Audio::Init()
{
    InitAudioDevice();

    SoundTrack = new raylib::Music();
    SoundTrack->looping = true;

    return true;
}

bool Audio::Deinit()
{
    SoundTrack->Unload();
    delete SoundTrack;

    for (auto& sound : SoundsMap)
    {
        sound.second->Stop();
        sound.second->Unload();
        delete sound.second;
    }

    SoundsMap.clear();

    CloseAudioDevice();

    return true;
}

void Audio::PlaySound(const std::string& soundFile)
{
    if (!SoundsMap.contains(soundFile))
        SoundsMap[soundFile] = new raylib::Sound();

    SoundsMap[soundFile]->Load(soundFile);

    if (SoundsMap[soundFile]->IsReady())
        SoundsMap[soundFile]->Play();
}

void Audio::PlayMusic(const std::string& musicFile, bool isLooping)
{
    SoundTrack->Load(musicFile.c_str());
    
    SoundTrack->looping = isLooping;

    if (SoundTrack->IsReady())
        SoundTrack->Play();
}

void Audio::StopMusic()
{
    SoundTrack->Stop();
}

void Audio::PauseMusic()
{
    //Pause = !Pause;

    //if (Pause) 
    if (SoundTrack->IsPlaying())
        SoundTrack->Pause();
    else 
        SoundTrack->Resume();
}

void Audio::Update()
{
    SoundTrack->Update();
}
