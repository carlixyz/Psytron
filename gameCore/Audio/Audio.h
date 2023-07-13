#pragma once

#include "../Utility/Singleton.h"

#include <iostream>
#include <map>

#include "Music.hpp"
#include "Sound.hpp"

class Audio : public Singleton<Audio>
{
	bool Pause = false;

	Music SoundTrack;

	std::map<std::string, raylib::Sound*> SoundsMap;

public:
	friend class Singleton<Audio>;

	bool Init();
	bool Deinit();

	void PlaySound(const std::string & soundFile);

	void PlayMusic(const std::string& musicFile, bool isLooping = true);
	void StopMusic();
	void PauseMusic();

	void Update();

protected:
	Audio() : SoundTrack() { ; }					// Protected Constructor

};

