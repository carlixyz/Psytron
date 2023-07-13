#include "Audio.h"


bool Audio::Init()
{
    InitAudioDevice();

    SoundTrack = LoadMusicStream("LP.mp3");

    return true;
}

bool Audio::Deinit()
{
    UnloadMusicStream(SoundTrack);

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
    {
        SoundsMap[soundFile] = new raylib::Sound();
        SoundsMap[soundFile]->Load(soundFile);
    }

    if (SoundsMap[soundFile]->IsReady())
        SoundsMap[soundFile]->Play();
}

void Audio::PlayMusic(const std::string& musicFile, bool isLooping)
{
    SoundTrack = LoadMusicStream(musicFile.c_str());
    
    SoundTrack.looping = isLooping;

    if (IsMusicReady(SoundTrack))
        PlayMusicStream(SoundTrack);
}

void Audio::StopMusic()
{
    StopMusicStream(SoundTrack);
}

void Audio::PauseMusic()
{
    Pause = !Pause;
    if (Pause) 
        if (IsMusicStreamPlaying(SoundTrack))
            PauseMusicStream(SoundTrack);
        else
            ResumeMusicStream(SoundTrack);
}

void Audio::Update()
{
    UpdateMusicStream(SoundTrack);
}
