#pragma once

#include <string>
//#include "../Utility/Utils.h"
//#include <assert.h>
//#include <yaml-cpp/yaml.h>

struct ApplicationProperties
{
	std::string ApplicationName = "Test 1";
	unsigned Width = 1080;
	unsigned Height = 720;
	unsigned Bits = 32;
	unsigned FPS = 60;
	bool Fullscreen = false;

	void LoadConfiguration(const std::string filename);
};

