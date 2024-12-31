#pragma once

#include <string>

struct ApplicationProperties
{
	std::string ApplicationName = "Test 1";
	std::string DialogFilePath	= "";

	unsigned Width				= 640;
	unsigned Height				= 480;
	unsigned Bits				= 32;
	unsigned FPS				= 60;
	float BulletSpeedFactor		= 1.0f;

	bool Fullscreen				= false;
	bool DebugMode				= false;
	bool SkipIntro				= false;

	void LoadConfiguration(const std::string filename);
};

