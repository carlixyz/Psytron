#pragma once
#include "GameState.h"
#include <raylib-cpp.hpp>
#include <vector>

class IntroState :  public GameState
{
public:

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

};