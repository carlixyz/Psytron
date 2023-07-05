#pragma once
#include "GameState.h"
//#include "../../Utility/Singleton.h"
#include <raylib-cpp.hpp>

class TitleState : public GameState//, public Singleton<TitleState>
{
	raylib::TextureUnmanaged logo;
	//raylib::Texture logo;

public:
	//friend class Singleton<IntroState>;

	void OnInit();
	void OnDeinit();

	void OnUpdate();
	void OnRender();

	//static TitleState* Instance()
	//{
	//	static TitleState introState;
	//	introState.allocated = false;
	//	return &introState;
	//}
};

