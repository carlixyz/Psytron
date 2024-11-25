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

	FontSize = 16 * (int)(Graphics::Get().GetFactorArea().y);

	MenuOptions.push_back("Introduction");
	MenuOptions.push_back(" Start Game ");
	MenuOptions.push_back("   Credits  ");


	credits.push_back("Idea & Programming");
	credits.push_back("Charlie");
	credits.push_back("Raylib framework");
	credits.push_back("Ramon Santamaria");
	credits.push_back("FX Sprites");
	credits.push_back("Renzo Maccarinni");
	credits.push_back("VN Intro CG");
	credits.push_back("Stable Diffusion");
	credits.push_back("Shmup logic");
	credits.push_back("ChatGPT");
	credits.push_back("Soundtrack");
	credits.push_back("Eric Matyas");
	credits.push_back("										Visit Soundimage.org");

}

void TitleState::OnDeinit()
{
	if (TitleLogo.IsReady())
		TitleLogo.Unload();
}

void TitleState::OnUpdate()
{
	if (!ShowCredits)
	{
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
			CurrentIndex--;
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
			CurrentIndex++;
		CurrentIndex = (int)Clamp((float)CurrentIndex, 0.f, MenuOptions.size() - 1.f);
	}

	if (CurrentTime < TotalTime)
	{
		CurrentTime += GetFrameTime();
		Alpha = EaseCircInOut(CurrentTime, 0.0f, 1.0f, TotalTime);
		return;
	}
		
	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
	{
		switch (CurrentIndex)
		{
			case 0:

				Game::Get().States.PushState(Game::Get().States.dialogState);
				break;
			case 1:	

				Game::Get().States.PushState(Game::Get().States.roadState);
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
		int i = 0, j = 0;

		const int LeftColumn = Graphics::Get().GetHorizontalCenter() - 200 * (int)(Graphics::Get().GetFactorArea().x);
		const int RightColumn = Graphics::Get().GetHorizontalCenter() + 100 * (int)(Graphics::Get().GetFactorArea().x);

		for (auto& credit : credits)
		{
			DrawText(credit.c_str(),
					 i % 2 == 0 ? LeftColumn : RightColumn,
					 Graphics::Get().GetVerticalCenter() + FontSize * j,
					 FontSize,
					 SKYBLUE );
			i++;
			j += (i % 2 == 0) ? 1 : 0;
		}

		return;
	}

	if (TitleLogo.IsReady())
		DrawTexturePro(TitleLogo, 
					   { 0, 0, 
					   (float)TitleLogo.width, 
					   (float)TitleLogo.height},
					   { Graphics::Get().GetHorizontalCenter() - TitleLogo.width * 0.5f * Graphics::Get().GetFactorArea().x,
					     Graphics::Get().GetVerticalCenter() - TitleLogo.height * 0.5f * Graphics::Get().GetFactorArea().y,
						TitleLogo.width * Graphics::Get().GetFactorArea().x,
						TitleLogo.height * Graphics::Get().GetFactorArea().y },
					   Vector2Zero(),
					   0.0f,
					   WHITE);
		//TitleLogo.Draw({ 0, 0 }, Fade(WHITE, Alpha));

	if ( Alpha >= 0.9f )
	{
		int i = 0;
		for (auto& option : MenuOptions)
		{
			DrawText(option.c_str(),
						Graphics::Get().GetHorizontalCenter() - ((int)(option.size() / 2 * FontSize / 2)),
						PADDING + Graphics::Get().GetVerticalCenter() + FontSize * i,
						FontSize,
						CurrentIndex == i++ ? Fade(WHITE, Alpha) : Fade(SKYBLUE, Alpha));
		}
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
