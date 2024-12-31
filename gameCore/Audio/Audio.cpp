#include "Audio.h"
#include <raylib-cpp.hpp>
#include "./reasings.h"

bool Audio::Init()
{
    InitAudioDevice();

    //SoundTrack = LoadMusicStream("LP.mp3");

    return true;
}

bool Audio::Deinit()
{
    //if (IsMusicReady(SoundTrack))
    //    UnloadMusicStream(SoundTrack);    
    /// Let's handle this from Assets Unloading to avoid more issues

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
        PreloadSound(soundFile);

    if (SoundsMap[soundFile]->IsReady())
        SoundsMap[soundFile]->Play();
}

void Audio::PreloadSound(const std::string& soundFile)
{
    if (!SoundsMap.contains(soundFile))
    {
        SoundsMap[soundFile] = new raylib::Sound();
        SoundsMap[soundFile]->Load(soundFile);
    }
}

void Audio::PlayMusic(const std::string& musicFile, bool isLooping)
{
    SoundTrack = LoadMusicStream(musicFile.c_str());
    SoundTrack.looping = isLooping;

    if (IsMusicReady(SoundTrack))
    {
        PlayMusicStream(SoundTrack);
        if (IsMusicFading || MusicVolumeNow <= 0.0f )
            FadeMusicIn();
    }
}

void Audio::PlayMusic( Music soundTrack, bool isLooping)
{
    SoundTrack = soundTrack;
    SoundTrack.looping = isLooping;

    if (IsMusicReady(SoundTrack))
    {
        PlayMusicStream(SoundTrack);
        if (IsMusicFading || MusicVolumeNow <= 0.0f)
            FadeMusicIn();
    }
}

void Audio::StopMusic()
{
    StopMusicStream(SoundTrack);
}

void Audio::FadeMusicIn()
{
    MusicVolumeStart = MusicVolumeNow;
    MusicVolumeEnd = 1.0f;
    CurrentTime = 0.0f;
    IsMusicFading = true;
}

void Audio::FadeMusicOut()
{
    MusicVolumeStart = MusicVolumeNow;
    MusicVolumeEnd = 0.0f;
    CurrentTime = 0.0f;
    IsMusicFading = true;
}

void Audio::ToggleMusic()
{
    Pause = !Pause;
    if (IsPlayingMusic())
        PauseMusicStream(SoundTrack);
    else
        ResumeMusicStream(SoundTrack);
}

bool Audio::IsPlayingMusic()
{
    return IsMusicStreamPlaying(SoundTrack);
}

void Audio::Update()
{
    if (IsMusicFading)
    {
        CurrentTime += GetFrameTime();
        MusicVolumeNow = EaseCubicOut(CurrentTime, MusicVolumeStart, MusicVolumeEnd - MusicVolumeStart, TotalTime);
        //MusicVolumeNow = EaseLinearNone(CurrentTime, MusicVolumeNow, MusicVolumeEnd - MusicVolumeNow, TotalTime);
        IsMusicFading = CurrentTime > TotalTime ? false : true;

        SetMusicVolume(SoundTrack, MusicVolumeNow);

        if (!IsMusicFading && MusicVolumeNow < 0.0f)
            StopMusicStream(SoundTrack);
    }


    UpdateMusicStream(SoundTrack);
}
