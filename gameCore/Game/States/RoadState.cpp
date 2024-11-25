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
	BGSource.y -= BGSpeed * GetFrameTime();
	RoadSource.y -= RoadSpeed * GetFrameTime();

	Particles::Get().OnUpdate();
	player->Update();
}

void RoadState::OnRender()
{
	CityBG->Draw(BGSource, BGDestiny);
	CityHighway->Draw(RoadSource, RoadDestiny);

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
