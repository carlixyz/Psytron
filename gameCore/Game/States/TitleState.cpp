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

	if (IsKeyPressed(KEY_C))
	{
		if(logo.IsReady())
			logo.Unload();
		//else
		//	logo.Load("george.png");
	}
}

void TitleState::OnRender()
{
	if (logo.IsReady())
		logo.Draw();

	//for (auto& image : Layers)
	//{
	//	if (image.IsReady())
	//		image.Draw();
	//}

	//DrawText("TITLE STATE", 190, 160, 40, RED);
}