#pragma once
#include "GameState.h"

class IntroState :  public GameState
{
public:

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

};