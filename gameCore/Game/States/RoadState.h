#pragma once


#include "GameState.h"
#include <raylib-cpp.hpp>

struct Player;
struct Enemy;
struct Bungie;

namespace raylib
{
	class TextureUnmanaged;
}

class RoadState : public GameState
{
public:
	//friend class Singleton<IntroState>;

	Player* player = nullptr;

	Enemy* enemy = nullptr;

	std::vector<Enemy*> Enemies;

	raylib::TextureUnmanaged* CityBG;
	raylib::TextureUnmanaged* CityHighway;

	raylib::Rectangle BGSource;
	raylib::Rectangle BGDestiny;
	float BGSpeed = 50;

	raylib::Rectangle RoadSource;
	raylib::Rectangle RoadDestiny;
	float RoadSpeed = 500;

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

	void OnPause();
	void OnResume();

private:
	void OnIntroUpdate();
	void OnIntroRender();


};

