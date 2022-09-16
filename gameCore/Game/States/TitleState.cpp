#include "TitleState.h"
#include "../Game.h"

void TitleState::OnInit()
{
	if(!logo.IsReady())
		logo.Load("raylib_logo.png");
}

void TitleState::OnDeinit()
{
	//logo.Unload();
}

void TitleState::OnUpdate()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		//Game::Get().PopState();
		//Game::Get().PushState(new IntroState());

		Game::Get().States.PopState();
		Game::Get().States.PushState(Game::Get().States.introState);

	}
}

void TitleState::OnRender()
{
	logo.Draw();

	//DrawText("TITLE STATE", 190, 160, 40, RED);
}