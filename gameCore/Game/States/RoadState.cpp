#include "RoadState.h"
#include "../../Graphics/Graphics.h"
#include "../Assets.h"
#include "../Shmup/Player.h"
#include "../Shmup/Enemy.h"
#include "../Shmup/Bullet.h"
#include "../Shmup/Particles.h"
#include "../Shmup/StepAction.h"
#include <list>


void RoadState::OnInit()
{
	player = new Player();
	Particles::Get().OnInit();
	Particles::Get().PlayerRef = player;

	std::vector<IStepAction*> StepActions = {
		new StepMoveTowards(Vector2(128, 256)),
		new StepShoot(BehaviourType::EArchShot, 2, 0.15f),
		new StepWait(5.0f),
		new StepShoot(BehaviourType::EAimShot, 5, 0.5f),
		new StepShoot(BehaviourType::ESeekShot, 5, 0.5f),
		//new StepShoot(BehaviourType::EHanaShot, 50, 0.25f),
		//new StepWait(1.0f),
		//new StepShoot(BehaviourType::EMultiSpiralShot, 30, 0.1f),
		new StepWait(1.0f),
		new StepMoveAbovePlayer(350.f),
		new StepWait(1.0f),
		new StepShoot(BehaviourType::ERingShot, 3, 1.0f),
		new StepShoot(BehaviourType::EStarShot, 1, 0.1f),
		new StepWait(1.5f),
		new StepMoveTowards(),
		new StepWait(1.5f),
		new StepShoot(BehaviourType::EMultiShot, 5, 0.15f),
		new StepWait(1.0f),
	};

	//Enemies.push_back(new Bungie(Graphics::Get().GetScreenCenter(), StepActions));
	EnemyProperties enemySettings;

	//Enemies.push_back(new Enemy(Graphics::Get().GetScreenCenter(), StepActions, Vector2(1,1)));
	Enemies.push_back(new Enemy(enemySettings, StepActions));
// 
	//Enemies.push_back(new Bungie());

	//enemy = new Enemy();

	//Data / Scenes / Track / Track_Side.png
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

	for (Enemy* enemy : Enemies)
	{
		delete enemy;
	}

	Enemies.clear();

}

void RoadState::OnUpdate()
{
	BGSource.y -= BGSpeed * GetFrameTime();
	RoadSource.y -= RoadSpeed * GetFrameTime();

	player->Update();

	for (Enemy* enemy : Enemies)
	{
		if (enemy->Active) 
			enemy->Update();
	}

	Particles::Get().OnUpdate(Enemies);
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


	OnIntroRender();
	
	Particles::Get().OnRender();

	player->Render();

	for (Enemy* enemy : Enemies)
	{
		if (enemy->Active)
			enemy->Render();
	}


}

void RoadState::OnPause()
{
}

void RoadState::OnResume()
{
}

void RoadState::OnIntroUpdate()
{
}

void RoadState::OnIntroRender()
{

	DrawTextureEx(Assets::Get().GetSprite("Truck"), Vector2(200, -200), 0.f, 2.f, WHITE);

	DrawTextureEx(Assets::Get().GetSprite("Prop"), Vector2(200, 200), 0.f, 2.f, WHITE);
}
