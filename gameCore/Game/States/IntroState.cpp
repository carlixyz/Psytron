#include "IntroState.h"
#include "../Game.h"

void IntroState::OnInit()
{
}

void IntroState::OnDeinit()
{
}

void IntroState::OnUpdate()
{

	//float deltaTime = GetFrameTime();
	if (IsKeyPressed(KEY_SPACE))
	{
		Game::Get().States.ChangeState(Game::Get().States.dialogState);
		//Game::Get().States.ChangeState(Game::Get().States.titleState);
		
		//Game::Get().States.PopState();
		//Game::Get().States.PushState(Game::Get().States.titleState);
	}
}

void IntroState::OnRender()
{
	//DrawText("INTRO STATE", 190, 250, 20, SKYBLUE);

}


//void IntroState::OnPause()
//{
//}

//void IntroState::OnResume()
//{
//}
