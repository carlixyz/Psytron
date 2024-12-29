#pragma once


#include "GameState.h"
#include <raylib-cpp.hpp>
#include "../Shmup/EnemyHandler.h"

struct Player;
struct Enemy;
//class EnemyHandler;

namespace raylib
{
	class TextureUnmanaged;
}

class RoadState : public GameState
{
public:
	//friend class Singleton<IntroState>; 

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

	void OnPause();
	void OnResume();


private:

	Player* player = nullptr;
	
	EnemyHandler Enemies;

	raylib::TextureUnmanaged* CityBG;
	raylib::TextureUnmanaged* CityHighway;

	raylib::Rectangle BGSource;
	raylib::Rectangle BGDestiny;
	float BGSpeed = 50;

	raylib::Rectangle RoadSource;
	raylib::Rectangle RoadDestiny;
	float RoadSpeed = 500;

	unsigned IntroState = 0;
	float CurrentFrameY = 0;
	float CurrentFrameX = 0;
	Vector2 TruckPosition = Vector2(200, Graphics::Get().GetWindowArea().height + 512);
	
	void OnIntroUpdate();
	void OnIntroRender();

	void OnHUDRender();

};

