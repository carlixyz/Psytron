#pragma once


#include "GameState.h"
#include <raylib-cpp.hpp>

struct Player;

namespace raylib
{
	class TextureUnmanaged;
}

class RoadState : public GameState
{
public:
	//friend class Singleton<IntroState>;

	Player* player;

	raylib::TextureUnmanaged* CityBG;
	raylib::TextureUnmanaged* CityHighway;

	raylib::Vector2 BGPosition;
	raylib::Rectangle BGOffset;
	float BGSpeed = 50;

	raylib::Vector2 RoadPosition;
	raylib::Rectangle RoadOffset;
	float RoadSpeed = 500;

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

	void OnPause();
	void OnResume();


};

