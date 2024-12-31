#pragma once

#include "../Utility/Singleton.h"
#include "../Graphics/ApplicationProperties.h"
#include "States/FSM.h"

class Game : public Singleton<Game>
{
	bool finish = false;

public:
	friend class Singleton<Game>;
	ApplicationProperties appProperties;
	FSM States;

	bool Init();
	void Update();

	void Render();
	bool Deinit();

	inline bool HasFinished()	{ return finish; }
	inline bool IsDebugMode()	{ return appProperties.DebugMode; }
	inline bool SkipIntro()		{ return appProperties.SkipIntro; }
	
	void AddScore()				{ Score += 100; }
	int GetScore()				{ return Score; }
	void ResetScore()			{ TopScore = Score;  Score = 0; }
	float GetShotSpeedFactor() { return appProperties.BulletSpeedFactor; }

protected:
	Game() { ; } // Protected Constructor

	int Score = 0;
	int TopScore = 0;
};

#define GetShotSpeedFactor() Game::Get().GetShotSpeedFactor()

