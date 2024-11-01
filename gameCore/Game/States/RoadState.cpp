#include "RoadState.h"
#include "../../Graphics/Graphics.h"
#include "../Shmup/Player.h"
#include "../Shmup/Bullet.h"
#include "../Shmup/Particles.h"
#include <list>


void RoadState::OnInit()
{
	player = new Player();
	Particles::Get().OnInit();
	Particles::Get().PlayerRef = player;

	//Data / Scenes / Track / Track_Side.png
	//CityBG = new raylib::TextureUnmanaged("Data/Scenes/Highway/MacroCityBG_Undot.png");
	//CityBG = new raylib::TextureUnmanaged("Data/Scenes/Highway/MacroCityBG_Blue_Dot.png");
	CityBG = new raylib::TextureUnmanaged("Data/Scenes/Highway/MacroCityBG_Blue_SmartDott.png");
	CityHighway = new raylib::TextureUnmanaged("Data/Scenes/Highway/MacroCityRoad.png");

	BGPosition = {(Graphics::Get().GetHorizontalCenter() - (CityBG->width * 0.5f)), 0.f};
	BGOffset = { 0, 0, (float)CityBG->width, (float)CityBG->height };

	RoadPosition = { (Graphics::Get().GetHorizontalCenter() - (CityHighway->width * 0.5f)), 0.f };
	RoadOffset = { 0, 0, (float)CityHighway->width, (float)CityHighway->height };
}

void RoadState::OnDeinit()
{
	Particles::Get().OnDeinit();
	Particles::Get().PlayerRef = nullptr;

	delete player;

	delete CityBG;
	delete CityHighway;
}

void RoadState::OnUpdate()
{
	BGOffset.y -= BGSpeed * GetFrameTime();
	RoadOffset.y -= RoadSpeed * GetFrameTime();
	Particles::Get().OnUpdate();
	player->Update();
}

void RoadState::OnRender()
{
	CityBG->Draw(BGOffset, BGPosition, WHITE);
	CityHighway->Draw(RoadOffset, RoadPosition, WHITE);

	DrawText("Made by Carlixyz \n Thanks to Fulanos",
			 Graphics::Get().GetHorizontalCenter() - 100,
			 100,
			 12,
			 SKYBLUE);

	Particles::Get().OnRender();


	player->Render();
}

void RoadState::OnPause()
{
}

void RoadState::OnResume()
{
}
