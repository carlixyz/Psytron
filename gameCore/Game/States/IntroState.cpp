#include "IntroState.h"
#include "../Game.h"
#include "./reasings.h"

void IntroState::OnInit()
{
	//* Add any of your logos
	Logos.push_back(new raylib::TextureUnmanaged("raylib_logo.png"));
	Logos.push_back(new raylib::TextureUnmanaged("Data/Sun.png"));
	//Game::Get().States.ChangeState(Game::Get().States.titleState);
}

void IntroState::OnDeinit()
{
	for (auto& logo : Logos)
	{
		logo->Unload();
		delete logo;
	}

	Logos.clear();
}

void IntroState::OnUpdate()
{
	CurrentTime += GetFrameTime();
	Alpha = EaseSineInOut(CurrentTime, 0.0f, 1.0f, TotalTime);

	if (IsKeyDown(KEY_SPACE))
	{
		CurrentTime += GetFrameTime() * 4;
	}

	if (CurrentTime > (TotalTime * 2))
	{
		CurrentTime = 0;
		CurrentLogo++;

		if (CurrentLogo >= Logos.size())
			Game::Get().States.ChangeState(Game::Get().States.titleState);
	}
}

void IntroState::OnRender()
{
	if(Logos[CurrentLogo] && Logos[CurrentLogo]->IsReady())
		Logos[CurrentLogo]->Draw({	(float)GetScreenWidth() * 0.5f - (float)(Logos[CurrentLogo]->GetWidth() * 0.5f),
									(float)GetScreenHeight() * 0.5f - (float)(Logos[CurrentLogo]->GetHeight() * 0.5f) },
									Fade(WHITE, Alpha));
}


//void IntroState::OnPause()
//{
//}

//void IntroState::OnResume()
//{
//}
