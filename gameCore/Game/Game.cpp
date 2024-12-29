#include "Game.h"

//#include "raylib-cpp.hpp"
#include "../Graphics/Graphics.h"
#include "../Audio/Audio.h"
#include "../Utility/Utils.h"
#include "ConversationManager.h"
#include "VisualDialogManager.h"
#include "Assets.h"

bool Game::Init()
{
	DEBUG_FILE("fileDebug.txt", DAY << PLACE << "Compilation number: " << GET_BUILD_NUM("Compilations.txt") << "\n");
	DEBUG_COUT("\n Last Compilation was " << GET_BUILD_NUM("Compilations.txt") << "\n\n");

	appProperties.LoadConfiguration("config.yml");
	bool result = Graphics::Get().Init(&appProperties);

	SetExitKey(KEY_Q);

	if (!result) Graphics::Get().Deinit();	// CloseWindow();	// If something fails Kill Window

	result = result && Assets::Get().Init();

	result = result && States.Init();							//	States.Init(States.introState);

	result = result && Audio::Get().Init();

	return result;
}

bool Game::Deinit()
{
	SET_BUILD_NUM("Compilations.txt");

	bool result = States.Deinit();								// cleanup the all states

	ConversationManager::Get().Deinit();

	VisualDialogManager::Get().Deinit();

	result = result && Assets::Get().Deinit();

	result = result && Audio::Get().Deinit();

	result = result && Graphics::Get().Deinit();

	return result;
}

void Game::Update()
{
	finish = (finish || Graphics::Get().GetCloseApplication());
	if (finish) return;

	// check for alt + enter
	if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
	{
		Graphics::Get().SwitchFullScreen();
	}

	//if(States.IsLoaded()) 
	States.CurrentState().OnUpdate();						/// statesStack.top()->OnUpdate(timeStep);

	VisualDialogManager::Get().Update();

	ConversationManager::Get().Update();

	Audio::Get().Update();
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);

	//if (States.IsLoaded())
	States.CurrentState().OnRender();						/// statesStack.top()->OnRender();

	VisualDialogManager::Get().Render();

	ConversationManager::Get().Render();

	EndDrawing();
}
