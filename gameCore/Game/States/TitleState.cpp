#include "TitleState.h"
#include "../Game.h"
#include "../../Graphics/Graphics.h"
#include "./reasings.h"

#define PADDING 100
#define SCREEN_CENTER_H (int)(Graphics::Get().GetWindowArea().width * 0.5f)
#define SCREEN_CENTER_V (int)(Graphics::Get().GetWindowArea().height * 0.5f)

void TitleState::OnInit()
{
	TitleLogo.Load("Data/PsyTronTitle.png");

	MenuOptions.push_back("Introduction");
	MenuOptions.push_back(" Start Game ");
	MenuOptions.push_back("   Credits  ");

}

void TitleState::OnDeinit()
{
	if (TitleLogo.IsReady())
		TitleLogo.Unload();
}

void TitleState::OnUpdate()
{
	if (CurrentTime < TotalTime)
	{
		CurrentTime += GetFrameTime();
		Alpha = EaseCircInOut(CurrentTime, 0.0f, 1.0f, TotalTime);
		return;
	}

	if (!ShowCredits)
	{
		if (IsKeyPressed(KEY_UP))
			CurrentIndex--;
		if (IsKeyPressed(KEY_DOWN))
			CurrentIndex++;
		CurrentIndex = (int)Clamp((float)CurrentIndex, 0.f, MenuOptions.size() - 1.f);
	}
		
	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
	{
		switch (CurrentIndex)
		{
			case 0:

				Game::Get().States.PushState(Game::Get().States.dialogState);
				break;
			case 1:	

				Game::Get().States.PushState(Game::Get().States.dialogState);
				break;
			case 2:
				ShowCredits = !ShowCredits;	// Toogle show Credits mode
				break;
		}
	}

}

void TitleState::OnRender()
{
	if (ShowCredits)
	{
		DrawText("Made by Carlixyz \n Thanks to Fulanos",
				 SCREEN_CENTER_H - 100,
				 PADDING + SCREEN_CENTER_V,
				 FontSize,
				 SKYBLUE);
		return;
	}

	if (TitleLogo.IsReady())
		TitleLogo.Draw({ 0, 0 }, Fade(WHITE, Alpha));

	int i = 0;
	for (auto& option : MenuOptions)
	{
		DrawText(option.c_str(),
					SCREEN_CENTER_H - ((int)(option.size() / 2 * FontSize / 2)),
					PADDING + SCREEN_CENTER_V + FontSize * i,
					FontSize,
					CurrentIndex == i++ ? Fade(WHITE, Alpha) : Fade(SKYBLUE, Alpha));
	}
}

void TitleState::OnPause()
{
}

void TitleState::OnResume()
{
	CurrentTime = 0.0f;
	Alpha = 0.0f;
}
