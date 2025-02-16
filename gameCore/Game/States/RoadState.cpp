#include "RoadState.h"
#include "../../Graphics/Graphics.h"
#include "../Assets.h"
#include "../Shmup/Player.h"
#include "../Shmup/Enemy.h"
#include "../Shmup/EnemyHandler.h"
#include "../Shmup/Bullet.h"
#include "../Shmup/Particles.h"
#include "../Shmup/StepAction.h"
#include "reasings.h"
#include <list>
#include "../../Audio/Audio.h"
#include <iomanip>
#include <sstream>
#include <string>


void RoadState::OnInit()
{
	player = new Player();
	player->Active = true;

	Particles::Get().OnInit();
	Particles::Get().PlayerRef = player;

	Enemies.Init();

	CityBG = new raylib::TextureUnmanaged("Data/Scenes/Highway/MacroCityBG_Blue_SmartDott.png");
	CityHighway = new raylib::TextureUnmanaged("Data/Scenes/Highway/MacroCityRoad.png");

	BGSource = { 0.f, 0.f, (float)CityBG->width, (float)CityBG->height };
	BGDestiny = { 
		(Graphics::Get().GetHorizontalCenter() - (CityBG->width * 0.5f) * Graphics::Get().GetFactorArea().x), 
		(Graphics::Get().GetVerticalCenter() - (CityBG->height * 0.5f) * Graphics::Get().GetFactorArea().y),
		CityBG->width* Graphics::Get().GetFactorArea().x, 
		CityBG->height * Graphics::Get().GetFactorArea().y 
	};

	RoadSource = { 0.f, 0.f, (float)CityHighway->width, (float)CityHighway->height };
	RoadDestiny = {
		(Graphics::Get().GetHorizontalCenter() - (CityHighway->width * 0.5f) * Graphics::Get().GetFactorArea().x),
		(Graphics::Get().GetVerticalCenter() - (CityHighway->height * 0.5f) * Graphics::Get().GetFactorArea().y),
		CityHighway->width * Graphics::Get().GetFactorArea().x,
		CityHighway->height * Graphics::Get().GetFactorArea().y
	};

	if (!PlayOnce)
	{
		PlayOnce = true;
		Audio::Get().PlayMusic(GetMusic("FireIntro"), false);
	}
	else
	{
		Audio::Get().PlayMusic(GetMusic("FireLoop"));
		Audio::Get().FadeMusicIn();
	}
	
}

void RoadState::OnDeinit()
{
	IntroState		= 0;
	CurrentFrameY	= 0;
	CurrentFrameX	= 0;
	TruckPosition	= Vector2(200, Graphics::Get().GetWindowArea().height + 512);

	Audio::Get().FadeMusicOut();

	Game::Get().ResetScore();

	delete CityHighway;
	delete CityBG;

	Enemies.Deinit();

	Particles::Get().OnDeinit();
	Particles::Get().PlayerRef = nullptr;
	delete player;
}

void RoadState::OnUpdate()
{
	BGSource.y -= BGSpeed * GetFrameTime();
	RoadSource.y -= RoadSpeed * GetFrameTime(); // *(Enemies.GetSpeedRequest() ? 0.75f : 1.0f);

	if (IntroState < 4)
		OnIntroUpdate();

	if (player->Active)
	{
		player->Update();
	}
	else if (IsKeyReleased(KEY_ESCAPE))
	{
		Game::Get().States.PopState();
	}

	Enemies.Update();

	Particles::Get().OnUpdate(Enemies.GetCurrentWave());
}


void RoadState::OnRender()
{
	CityBG->Draw(BGSource, BGDestiny);
	CityHighway->Draw(RoadSource, RoadDestiny);

	if (player->Active)
		player->Render();

	if (IntroState < 4)
		OnIntroRender();

	Particles::Get().OnRender();

	Enemies.Render();

	OnHUDRender();
}

void RoadState::OnPause()
{
	Audio::Get().ToggleMusic();

}

void RoadState::OnResume()
{
	Audio::Get().ToggleMusic();
}

void RoadState::OnIntroUpdate()
{
	if (!Audio::Get().IsPlayingMusic())
		Audio::Get().PlayMusic(GetMusic("FireLoop"));

	if (player)
	{
		CurrentFrameY += GetFrameTime();
		CurrentFrameX += GetFrameTime();

		if (IntroState == 0)
		{
			player->LeftMargin = 192 * Graphics::Get().GetFactorArea().x;

			TruckPosition.y = EaseCircIn(CurrentFrameY, TruckPosition.y, 1 - TruckPosition.y, 10.0f);

			if (TruckPosition.y < 2)
				IntroState = 1;
		}
		else if (IntroState == 1)
		{
			TruckPosition.y = EaseSineInOut(CurrentFrameY, -128, 128, 4.0f);

			if (Enemies.GetCurrentWaveIndex() > 2)
			{
				CurrentFrameY = 0;
				IntroState = 2;
			}
		}
		else if (IntroState == 2)
		{
			TruckPosition.y = EaseCubicIn(CurrentFrameY, TruckPosition.y,
											  (Graphics::Get().GetWindowArea().height + 32) - TruckPosition.y, 20.f);

			if (TruckPosition.y > Graphics::Get().GetScreenCenter().y)
			{
				player->LeftMargin = 86 * Graphics::Get().GetFactorArea().x;
				IntroState++;
			}
		}
		else if (IntroState > 2)
		{
			TruckPosition.y = EaseCubicIn(CurrentFrameY, TruckPosition.y,
										  (Graphics::Get().GetWindowArea().height + 32) - TruckPosition.y, 20.f);

			if (TruckPosition.y > Graphics::Get().GetWindowArea().height)
				IntroState = 4;
		}
			
		TruckPosition.x = EaseSineInOut(CurrentFrameX, 
										100 * Graphics::Get().GetFactorArea().x,
										(75 - 100) * Graphics::Get().GetFactorArea().x,
										5.0f);

		if (player->IntroMode)
		{
			player->Position.y = EaseCircIn(CurrentFrameY, player->Position.y,
											Graphics::Get().GetScreenCenter().y - player->Position.y, 20.0f);
			
			if (player->Position.y < Graphics::Get().GetScreenCenter().y + 128)
				player->IntroMode = false;
		}

	}
}

void RoadState::OnIntroRender()
{
	DrawTextureEx(Assets::Get().GetSprite("Truck"), TruckPosition, 0.f, Graphics::Get().GetFactorArea().y, WHITE);
}

void RoadState::OnHUDRender()
{

	DrawText("SCORE", 16, 16, 24, RAYWHITE);

	std::stringstream stream;
	stream << " " << std::fixed << std::setprecision(2) << Game::Get().GetScore() << " ";
	std::string Text = stream.str();
	DrawText(Text.c_str(), 16, 50, 24, RAYWHITE);

	//---------------------------------------------------------------------------------------------
	DrawText("SHIELD", 16, (int)Graphics::Get().GetWindowArea().height - 64, 24, RAYWHITE);

	if (player->Active)
		for (int i = 0; i < player->Shield; i++)
		{
			DrawRectangleGradientV(16 + (17 * i), (int)Graphics::Get().GetWindowArea().height - 96, 16, 24, RED, RAYWHITE);
		}
	else
		DrawText("GAME OVER", (int)Graphics::Get().GetScreenCenter().x -128,
				 (int)Graphics::Get().GetScreenCenter().y, 48, RED);
}

