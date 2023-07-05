#include "FSM.h"

bool FSM::Init(GameState& state)
{
	PushState(state);

	return statesStack.size();
}

bool FSM::Init()
{
	PushState(introState);

	return statesStack.size();
}

bool FSM::Deinit()
{
	while (!statesStack.empty())		// cleanup the all states
	{
		statesStack.top()->OnDeinit();
		//delete statesStack.top();
		statesStack.pop();
	}

	return statesStack.empty();
}

void FSM::ChangeState(GameState& state)
{
	if (!statesStack.empty())			// if not Empty CleanUp current State				
	{
		statesStack.top()->OnDeinit();
		statesStack.pop();
	}

	statesStack.push(&state);
	statesStack.top()->OnInit();
}

void FSM::PushState(GameState& state)
{
	if (!statesStack.empty())
		statesStack.top()->OnPause();	// pause current state				

	statesStack.push(&state);
	statesStack.top()->OnInit();
}

void FSM::PopState()
{
	if (statesStack.size() > 0)			// cleanup the current state
	{
		statesStack.top()->OnDeinit();
		statesStack.pop();
	}

	if (!statesStack.empty())			// resume previous state	
		statesStack.top()->OnResume();
}
