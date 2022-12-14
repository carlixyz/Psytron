#include "Game.h"

//#include "raylib-cpp.hpp"
#include "../Graphics/Graphics.h"
#include "../Utility/Utils.h"
#include "ConversationManager.h"

bool Game::Init()
{
	DEBUG_FILE("fileDebug.txt", DAY << PLACE << "Compilation number: " << GET_COMP_NUM("Compilations.txt") << "\n");
	DEBUG_COUT("\n Last Compilation was " << GET_COMP_NUM("Compilations.txt") << "\n\n");

	appProperties.LoadConfiguration("config.yml");
	bool result = Graphics::Get().Init(&appProperties);

	if (!result) Graphics::Get().Deinit();	// CloseWindow();	// If something fails Kill Window

	SetExitKey(KEY_Q);

	ConversationManager::Get().Init("Data/dialogTest.yml");
	ConversationManager::Get().StartConversation("Comienzo"); // Activar este para realizar una comica charla al iniciar el juego
	//ConversationManager::Get().StartConversation("Setup"); // Activar este para realizar una comica charla al iniciar el juego


	result = result && States.Init();							//	States.Init(States.introState);

	return result;
}

bool Game::Deinit()
{
	SET_COMP_NUM("Compilations.txt");

	bool result = States.Deinit();								// cleanup the all states

	ConversationManager::Get().Deinit();

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

	ConversationManager::Get().Update();
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);

	//if (States.IsLoaded())
		States.CurrentState().OnRender();						/// statesStack.top()->OnRender();

	//DrawText("Congrats! Your Game is up & running!", 190, 200, 20, LIGHTGRAY);
	
		ConversationManager::Get().Render();


	EndDrawing();
}
