#pragma once
#include "GameState.h"
//#include "../../Utility/Singleton.h"
#include <raylib-cpp.hpp>
#include <list>

class TitleState : public GameState//, public Singleton<TitleState>
{
	raylib::TextureUnmanaged TitleLogo;
	//raylib::Texture logo;
	float Alpha = 0.0f;							// Current Alpha Color Value
	float StartValue = 0.f;
	float EndValue = 1.0f;
	float CurrentTime = 0.0f;
	float TotalTime = 3.0f;

	int CurrentIndex = 0;
	int MaxIndex = 3;
	int FontSize = 16;
	std::list<std::string> MenuOptions;

	bool ShowCredits = false;
	std::list<std::string> credits;

public:
	//friend class Singleton<IntroState>;

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

	void OnPause();
	void OnResume();

	//static TitleState* Instance()
	//{
	//	static TitleState introState;
	//	introState.allocated = false;
	//	return &introState;
	//}
};

