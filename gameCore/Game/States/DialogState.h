#pragma once
#include "GameState.h"
#include <raylib-cpp.hpp>

class DialogState : public GameState
{
public:

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

};

