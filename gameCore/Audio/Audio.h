#pragma once

#include "../Utility/Singleton.h"

#include <iostream>
#include <map>

#include "Music.hpp"
#include "Sound.hpp"

class Audio : public Singleton<Audio>
{
	bool Pause = false;
	bool IsMusicFading = false;
	float CurrentTime = 0.0f;
	float TotalTime = 5.0f;

	float MusicVolumeNow = 1.0f;
	float MusicVolumeEnd = 0.0f;
	float MusicVolumeStart = 0.0f;

	Music SoundTrack;

	std::map<std::string, raylib::Sound*> SoundsMap;


public:
	friend class Singleton<Audio>;

	bool Init();
	bool Deinit();

	void PlaySound(const std::string & soundFile);

	void PlayMusic(const std::string& musicFile, bool isLooping = true);
	void StopMusic();
	void FadeMusicIn();
	void FadeMusicOut();

	void ToggleMusic();

	bool IsPlayingMusic();

	void Update();

protected:
	Audio() : SoundTrack() { ; }					// Protected Constructor


};

