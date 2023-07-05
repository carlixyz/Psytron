#pragma once

#include <stack>
#include "GameState.h"
#include "DialogState.h"
#include "IntroState.h"
#include "TitleState.h"
//#include "../../Utility/Utils.h"
#include <assert.h>

class FSM	/// Finite State Machine ftw!
{
	std::stack<GameState*> statesStack; 

public:
	bool Init(GameState& state);
	bool Init();
	bool Deinit();

	void ChangeState(GameState& state);
	void PushState(GameState& state);
	void PopState();
	
	//inline bool IsLoaded()				{ return !statesStack.empty(); }
	inline GameState& CurrentState()	
	{
		assert(!statesStack.empty());
		return *statesStack.top(); 
	}

	/// All states instances included below here

	DialogState dialogState;
	IntroState introState;
	TitleState titleState;
};

