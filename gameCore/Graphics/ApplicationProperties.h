#pragma once

#include <string>

struct ApplicationProperties
{
	std::string ApplicationName = "Test 1";
	std::string DialogFilePath = "";

	unsigned Width = 640;
	unsigned Height = 480;
	unsigned Bits = 32;
	unsigned FPS = 60;
	bool Fullscreen = false;


	void LoadConfiguration(const std::string filename);
};

