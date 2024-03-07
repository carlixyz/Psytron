#pragma once
#include "GameState.h"
#include <raylib-cpp.hpp>
#include <vector>

class BootState :  public GameState
{
	float Alpha			= 0.0f;					// Current Alpha Color Value
	float StartValue	= 0.f;
	float EndValue		= 1.0f;
	float CurrentTime	= 0.0f;
	float TotalTime		= 3.0f;

	int CurrentLogo		= 0;
	std::vector<raylib::TextureUnmanaged*> Logos;

public:

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

};