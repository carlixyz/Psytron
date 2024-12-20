#include "BootState.h"
#include "../Game.h"
#include "../../Graphics/Graphics.h"
#include "./reasings.h"

void BootState::OnInit()
{
	//* Add any of your logos
	Logos.push_back(new raylib::TextureUnmanaged("raylib_logo.png"));
	Logos.push_back(new raylib::TextureUnmanaged("Data/DaliDavros.png"));
	Logos.push_back(new raylib::TextureUnmanaged("Data/Sun.png"));
	Logos.push_back(new raylib::TextureUnmanaged("Data/InderFort.png"));
	Logos.push_back(new raylib::TextureUnmanaged("Data/NazGaona.png"));
	//Game::Get().States.ChangeState(Game::Get().States.titleState);
}

void BootState::OnDeinit()
{
	for (auto& logo : Logos)
	{
		logo->Unload();
		delete logo;
	}

	Logos.clear();
}

void BootState::OnUpdate()
{
	CurrentTime += GetFrameTime();
	Alpha = EaseSineInOut(CurrentTime, 0.0f, 1.0f, TotalTime);

	if (IsKeyDown(KEY_ENTER))
	{
		Game::Get().States.ChangeState(Game::Get().States.titleState);
		return;
	}

	if (IsKeyDown(KEY_SPACE))
	{
		CurrentTime += GetFrameTime() * 4;
	}

	if (CurrentTime > (TotalTime * 2))
	{
		CurrentTime = 0;
		CurrentLogo++;

		if (CurrentLogo >= Logos.size())		
			Game::Get().States.ChangeState(Game::Get().States.introState);
	}
}

void BootState::OnRender()
{
	if (Logos[CurrentLogo] && Logos[CurrentLogo]->IsReady())
	{
		const Vector2 Position = { 
			Graphics::Get().GetHorizontalCenter() - (float)Logos[CurrentLogo]->GetWidth() * 0.5f * Graphics::Get().GetFactorArea().x,
			Graphics::Get().GetVerticalCenter() - (float)Logos[CurrentLogo]->GetHeight() * 0.5f * Graphics::Get().GetFactorArea().y 
		};

		Logos[CurrentLogo]->Draw( Position, 0.0f, Graphics::Get().GetFactorArea().x, Fade(WHITE, Alpha));
	}
}


//void BootState::OnPause()
//{
//}

//void BootState::OnResume()
//{
//}
