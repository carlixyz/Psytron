#pragma once

#include "../Utility/Singleton.h"
#include "../Graphics/Graphics.h"
#include "States/FSM.h"

class Game : public Singleton<Game>
{
	bool finish = false;
	ApplicationProperties appProperties;

public:
	friend class Singleton<Game>;
	FSM States;

	bool Init();
	void Update();

	void Render();
	bool Deinit();

	inline bool HasFinished() { return finish; }

protected:
	Game() { ; } // Protected Constructor
};

